#include "Console.h"
#include "Button.h"
#include "Input.h"
#include "Menu.h"
#include <iostream>
#include <thread>
#include "UserInfo.h"
#include <algorithm>
#include "Popup.h"
#include <limits>

#ifdef max
#undef max
#endif

void ShowConfiguration();
void ShowMain();

int main()
{ 
  Console::Resize(300, 300);
  HWND window = GetConsoleWindow();
  SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
  Console::ShowConsoleCursor(false);
  Console::SetTitle("password manager");
  ShowMain();
  while (true)
  {
    if (Menu::Enabled())
    {
      FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
      if (Input::GetKeyDown(VK_DOWN))
        Menu::NextDown();
      if (Input::GetKeyDown(VK_UP))
        Menu::NextUp();
      if (Input::GetKeyDown(VK_RETURN))
        Menu::Press();
    }
    else
    {
      Console::SetCursorPosition(Menu::InputX(), Menu::InputY());
      std::string input = Console::ReadLine();
      if (!input.empty())
      {
        Menu::ExecuteInputCallback(input);
        std::cin.clear();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
      }
    }
  }
  return 0;
}

void NewCategory()
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ new category ]", Color::LIGHT_AQUA);
  Console::WriteLine("");
  Console::WriteLineCentered("Category Name", Color::LIGHT_GREEN);
  Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
  Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
  Menu::Enabled() = false;
  Console::ShowConsoleCursor(true);
  Menu::SetInputCallback([] (const std::string& name) 
  {
    Menu::Enabled() = true;
    Console::ShowConsoleCursor(false);
    if (std::any_of(UserInfo::Categories().begin(), UserInfo::Categories().end(), [&] (Category& x) { return x.name == name; }))
    {
      Popup(1000, 8, 20, 3, "Already Exists", Color::WHITE, Color::LIGHT_RED);
      ShowConfiguration();
    }
    else
    {
      UserInfo::Categories().push_back({ name, std::vector<Entry>() });
      Popup(1000, 8, 20, 3, "Category Added", Color::WHITE, Color::LIGHT_GREEN);
      ShowConfiguration();
    }
  }, (Console::GetSize().X - 16) / 2, 4);
}

void DeleteCategory()
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ delete category ]", Color::LIGHT_RED);
  Console::WriteLine("");
  Console::WriteLineCentered("Select Category", Color::LIGHT_GREEN);
  int i = 0;
  for (Category& c : UserInfo::Categories())
  {
    Menu::Buttons().emplace_back(0, (i++) + 4, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, c.name, c.name, c.name, [&] ()
    { 
      UserInfo::Categories().erase(std::find_if(UserInfo::Categories().begin(), UserInfo::Categories().end(), [&] (Category& x) { return x.name == c.name; }));
      Popup(1000, 8, 20, 3, "Category Deleted", Color::WHITE, Color::LIGHT_GREEN);
      ShowConfiguration(); 
    }, [] () {}, true);
  }
  Menu::Buttons().emplace_back(0, i + 5, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Back", "Back", "Back", ShowConfiguration, [] () {}, true);
  Menu::Buttons()[0].Select();
}

void CreateEntry(Category& category)
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
  Console::WriteLine("");
  Console::WriteLineCentered("Entry Name", Color::LIGHT_GREEN);
  Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
  Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
  Menu::Enabled() = false;
  Console::ShowConsoleCursor(true);
  Menu::SetInputCallback([&] (const std::string& name) 
  {
    Menu::Enabled() = true;
    Console::ShowConsoleCursor(false);
    if (std::any_of(category.entries.begin(), category.entries.end(), [&] (Entry& x) { return x.name == name; }))
    {
      Popup(1000, 8, 20, 3, "Already Exists", Color::WHITE, Color::LIGHT_RED);
      ShowConfiguration();
    }
    else
    {
      category.entries.emplace_back();
      Entry& entry = category.entries.back();
      entry.name = name;
      Menu::Clear();
      Console::WriteLine("");
      Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
      Console::WriteLine("");
      Console::WriteLineCentered("Username (N for none)", Color::LIGHT_GREEN);
      Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
      Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
      Menu::Enabled() = false;
      Console::ShowConsoleCursor(true);
      Menu::SetInputCallback([&] (const std::string& username) 
      {
        Menu::Enabled() = true;
        Console::ShowConsoleCursor(false);
        entry.username = (username == "N") || (username == "n") ? "" : username;
        Menu::Clear();
        Console::WriteLine("");
        Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
        Console::WriteLine("");
        Console::WriteLineCentered("Email (N for none)", Color::LIGHT_GREEN);
        Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
        Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
        Menu::Enabled() = false;
        Console::ShowConsoleCursor(true);
        Menu::SetInputCallback([&] (const std::string& email) 
        {
          Menu::Enabled() = true;
          Console::ShowConsoleCursor(false);
          entry.email = (email == "N") || (email == "n") ? "" : email;
          Menu::Clear();
          Console::WriteLine("");
          Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
          Console::WriteLine("");
          Console::WriteLineCentered("Password", Color::LIGHT_GREEN);
          Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
          Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
          Menu::Enabled() = false;
          Console::ShowConsoleCursor(true);
          Menu::SetInputCallback([&] (const std::string& password)
          {
            Menu::Enabled() = true;
            Console::ShowConsoleCursor(false);
            entry.password = password;
            Menu::Clear();
            Console::WriteLine("");
            Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
            Console::WriteLine("");
            Console::WriteLineCentered("URL", Color::LIGHT_GREEN);
            Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
            Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
            Menu::Enabled() = false;
            Console::ShowConsoleCursor(true);
            Menu::SetInputCallback([&] (const std::string& url)
            {
              Menu::Enabled() = true;
              Console::ShowConsoleCursor(false);
              entry.url = url;
              Menu::Clear();
              Console::WriteLine("");
              Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
              Console::WriteLine("");
              Console::WriteLineCentered("Note", Color::LIGHT_GREEN);
              Console::ColorLineCentered(Color::LIGHT_GREEN, 16);
              Console::SetColor(Color::LIGHT_PURPLE, Color::LIGHT_GREEN);
              Menu::Enabled() = false;
              Console::ShowConsoleCursor(true);
              Menu::SetInputCallback([&] (const std::string& note) 
              {
                Menu::Enabled() = true;
                Console::ShowConsoleCursor(false);
                entry.note = note;
                Popup(1000, 8, 20, 3, "Entry Added", Color::WHITE, Color::LIGHT_GREEN);
                ShowConfiguration();
              }, (Console::GetSize().X - 16) / 2, 4);
            }, (Console::GetSize().X - 16) / 2, 4);
          }, (Console::GetSize().X - 16) / 2, 4);
        }, (Console::GetSize().X - 16) / 2, 4);
      }, (Console::GetSize().X - 16) / 2, 4);
    }
  }, (Console::GetSize().X - 16) / 2, 4);
}

void NewEntry()
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ new entry ]", Color::LIGHT_AQUA);
  Console::WriteLine("");
  Console::WriteLineCentered("Select Category", Color::LIGHT_GREEN);
  int i = 0;
  for (Category& c : UserInfo::Categories())
  {
    Menu::Buttons().emplace_back(0, (i++) + 4, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, c.name, c.name, c.name, [&] () { CreateEntry(c); }, [] () {}, true);
  }
  Menu::Buttons().emplace_back(0, i + 5, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Back", "Back", "Back", ShowConfiguration, [] () {}, true);
  Menu::Buttons()[0].Select();
}

void ShowConfiguration()
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ config ]", Color::LIGHT_AQUA);
  Menu::AddButtons(std::vector<Button>
  {
      Button(0, 3, Color::WHITE, Color::BLACK, Color::BLACK, Color::LIGHT_GREEN, Color::LIGHT_PURPLE, "New Category", "New Category", "New Category", NewCategory, [] () {}, true),
      Button(0, 4, Color::WHITE, Color::BLACK, Color::BLACK, Color::LIGHT_RED, Color::LIGHT_PURPLE, "Delete Category", "Delete Category", "Delete Category", DeleteCategory, [] () {}, true),
      Button(0, 5, Color::WHITE, Color::BLACK, Color::BLACK, Color::LIGHT_GREEN, Color::LIGHT_PURPLE, "New Entry", "New Entry", "New Entry", NewEntry, [] () {}, true),
      Button(0, 6, Color::WHITE, Color::BLACK, Color::BLACK, Color::LIGHT_YELLOW, Color::LIGHT_PURPLE, "Edit Entry", "Edit Entry", "Edit Entry", [] () {}, [] () {}, true),
      Button(0, 7, Color::WHITE, Color::BLACK, Color::BLACK, Color::LIGHT_RED, Color::LIGHT_PURPLE, "Delete Entry", "Delete Entry", "Delete Entry", [] () {}, [] () {}, true),
      Button(0, 8, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Back", "Back", "Back", ShowMain, [] () {}, true)
  });
  Menu::Buttons()[0].Select(true);
}

void ShowMain()
{
  Menu::Clear();
  Console::WriteLine("");
  Console::WriteLineCentered("[ password manager ]", Color::LIGHT_AQUA);
  Console::WriteLineCentered("program and api by: cry", Color::LIGHT_AQUA);
  Menu::AddButtons(std::vector<Button>
  {
      Button(0, 4, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Categories", "Categories", "Categories", [] () {}, [] () {}, true),
      Button(0, 5, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Configuration", "Configuration", "Configuration", ShowConfiguration, [] () {}, true),
      Button(0, 6, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "License", "License", "License", [] () {}, [] () {}, true),
      Button(0, 7, Color::WHITE, Color::BLACK, Color::BLACK, Color::WHITE, Color::LIGHT_PURPLE, "Exit", "Exit", "Exit", [] () { exit(0); }, [] () {}, true)
  });
  Menu::Buttons()[0].Select(true);
}
