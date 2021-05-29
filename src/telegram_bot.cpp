#include "telegram.h"

#include <fstream>
#include <wordexp.h>


using namespace telegram;
using namespace std;

const string telegram::Bot::default_token_path = "$HOME/.bitrader/telegrambot/token";
const string telegram::Bot::default_chatid_path = "$HOME/.bitrader/telegrambot/chatid";

telegram::Bot::Bot(const string token_, const unsigned long chatid_) :

token("1764216049:AAGeC6Qfnp1PD_uV3weMjuZ4NfLGAFhd6AQ"), chatid(1025988299)
{
	if (token == "")
	{
	    cout << "hello" << endl;
		wordexp_t p;
		char** w;
		wordexp(default_token_path.c_str(), &p, 0);
		w = p.we_wordv;
		ifstream telegrambot(w[0]);
		if (telegrambot.is_open())
		{
			telegrambot >> token;
			telegrambot.close();
		}
		wordfree(&p);
	}

	if (chatid == 0)
	{
		wordexp_t p;
		char** w;
		wordexp(default_chatid_path.c_str(), &p, 0);
		w = p.we_wordv;
		ifstream telegrambot(w[0]);
		if (telegrambot.is_open())
		{
			telegrambot >> chatid;
			telegrambot.close();
		}
		wordfree(&p);
	}
}

bool telegram::Bot::keysAreSet() const
{
	return ((token != "") && (chatid != 0));
}

telegramError_t telegram::Bot::initialize()
{
	telegramError_t status = telegramSuccess;
	cout << "successfully initialized telegram instance" << endl;

	if (!bot.get())
	{
		try
		{
			bot.reset(new TgBot::Bot(token.c_str()));
		}
		catch (TgBot::TgException& e)
		{
		    status = telegramErrorInitializationFailed;
		}
	}

	cout << "setting webhook" << endl;
//    bot->getApi().setWebhook("https:///hooks/", TgBot::InputFile::fromFile("cert.pem", "application/x-pem-file"), 40);
	cout << "status is " << status << endl;
	
	return status;
}

telegramError_t telegram::Bot::sendMessage(string message)
{
	telegramError_t status = initialize();
	
	if (status != telegramSuccess)
		return status;

	try
	{
		bool disableWebPagePreview = true;
		cout << "trying to send a message : " << message << endl;

		// First, send queued messages, if any.
//		if (msgQueue.size())
//		{
//			for (int i = 0, e = msgQueue.size(); i < e; i++)
//			{
//				bot->getApi().sendMessage(chatid, msgQueue.front(), disableWebPagePreview, 0, TgBot::GenericReply::Ptr(), "HTML");
//				msgQueue.pop();
//			}
//		}
				
		bot->getApi().sendMessage(chatid, message, disableWebPagePreview, 0, TgBot::GenericReply::Ptr(), "HTML");
	}
	catch (TgBot::TgException& e)
	{
		status = telegramErrorSendMessageFailed;
		cout << "not sended & status " << status << endl;

		// Store message in the queue to repeat the sending
		// attempt next time.
		msgQueue.push(message);
	}

	return status;
}

