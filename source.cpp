#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <tgbot/tgbot.h>

//детект куар кодов с помощью опенсв
//генерирование куар кодов на введенное слово/видео/линку


int max_size = 20480; // 20 Mbytes
int max_duration = 30; // 30 seconds
std::string token = "YOUR TOKEN";

int main() {
    TgBot::Bot bot(token);
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi " + message->chat->firstName + "!");
    });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);


        if (!message->photo.empty()) {
                auto file_id = message->photo[0]->fileId;
                auto file_info = bot.getApi().getFile(message->photo[message->photo.size() - 1]->fileId);
                auto downloaded = bot.getApi().downloadFile(file_info->filePath);

                std::ofstream out("image.jpg", std::ios::binary);
                out << downloaded;
            }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });
    


    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
    

    return 0;
}
