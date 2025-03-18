#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

#include <tgbot/tgbot.h>

#include <chrono>  // для функций из std::chrono
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;
using namespace TgBot;

string n;

int main_wol(int argc, char* argv[]);

void processCommand(Bot* bot, Message::Ptr message)
{
  printf("User wrote %s\n", message->text.c_str());

  if (message->text == "/wakesrv") {
    string name_str = "tg_wakeup_pc";
    string mac_str = "22:22:5c:06:0d:06";
    char* arg_ch_ptr[2];
    arg_ch_ptr[0] = name_str.data();
    arg_ch_ptr[1] = mac_str.data();
    main_wol(2, arg_ch_ptr);
  }

  if (StringTools::startsWith(message->text, "/start"))
    return;
  bot->getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
}



int main()
{
  string token(getenv("TOKEN"));
  printf("Token: %s\n", token.c_str());

  Bot bot(token);
  bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Hi!");
  });

  bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
    processCommand(&bot, message);
  });

  signal(SIGINT, [](int s) {
    printf("SIGINT got: %i\n", s);
    exit(0);
  });

  try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    TgLongPoll longPoll(bot);
    while (true) {
      printf("Long poll started\n");
      longPoll.start();
    }
  } catch (exception &e) {
    printf("error: %s\n", e.what());
  }

  return 0;
}
