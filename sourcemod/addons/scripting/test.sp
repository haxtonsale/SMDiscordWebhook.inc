#include <sourcemod>
#include <profiler>

//#define DISCORDWEBHOOK_USE_STEAMWORKS
//#define DISCORDWEBHOOK_USE_STEAMTOOLS
#include <discordwebhook>

#define ID1
#define TOKEN1

#define ID2
#define TOKEN2

#define ID3
#define TOKEN3

#define ID4
#define TOKEN4

#define ID5
#define TOKEN5

Profiler g_Profiler;

public void OnPluginStart()
{
	g_Profiler = new Profiler();
	g_Profiler.Start();
	DiscordWebhook_Execute(ID1, TOKEN1, "Test message!", OnDWResponseReceived, "Replaced username", "https://sun9-28.userapi.com/c854428/v854428793/1be6dc/gFNtrILIl1A.jpg");
	g_Profiler.Stop();
	PrintToServer("DiscordWebhook_Execute took %f ms", g_Profiler.Time*1000.0);
	
	g_Profiler.Start();
	DiscordWebhook_ExecuteRaw(ID2, TOKEN2, "application/json", "{\"content\": \"Test message!\", \"embeds\": [{\"title\": \"Test!\"}, {\"title\": \"Test 2!\"}]}", OnDWResponseReceived);
	g_Profiler.Stop();
	PrintToServer("DiscordWebhook_ExecuteRaw took %f ms", g_Profiler.Time*1000.0);
	
	TestJson(ID3, TOKEN3);
	TestJson(ID4, TOKEN4);
	TestJsonWithMethodmaps();
}

void TestJson(const char[] id, const char[] token)
{
	g_Profiler.Start();
	
	JSON_Array embeds = new JSON_Array();
	
	JSON_Object embed = new JSON_Object();
	embed.SetString("title", "Test!");
	embed.SetString("description", "Description");
	embed.SetString("url", "https://github.com/haxtonsale");
	embed.SetString("timestamp", "2020-01-10T18:10:00.000Z");
	embed.SetInt("color", 0x6E14FF);
	
	JSON_Object footer = new JSON_Object();
	footer.SetString("text", "Footer text");
	footer.SetString("icon_url", "https://sun9-55.userapi.com/c543105/v543105788/6190f/vO_rcer0dz8.jpg");
	embed.SetObject("footer", footer);
	
	JSON_Object image = new JSON_Object();
	image.SetString("url", "https://sun9-56.userapi.com/c543105/v543105102/6f1ba/HvB5HmGHPCc.jpg");
	embed.SetObject("image", image);
	
	JSON_Object thumbnail = new JSON_Object();
	thumbnail.SetString("url", "https://sun1-88.userapi.com/c856524/v856524904/aa1f6/IRHKoE_2_Ng.jpg");
	embed.SetObject("thumbnail", thumbnail);
	
	JSON_Object author = new JSON_Object();
	author.SetString("name", "Haxton Sale");
	author.SetString("url", "https://github.com/haxtonsale");
	author.SetString("icon_url", "https://avatars2.githubusercontent.com/u/26278199");
	embed.SetObject("author", author);
	
	JSON_Array fields = new JSON_Array();
	JSON_Object field1 = new JSON_Object();
	field1.SetString("name", "Field 1");
	field1.SetString("value", "Text");
	field1.SetBool("inline", true);
	JSON_Object field2 = new JSON_Object();
	field2.SetString("name", "Field 2");
	field2.SetString("value", "Text");
	field2.SetBool("inline", true);
	JSON_Object field3 = new JSON_Object();
	field3.SetString("name", "Field 3");
	field3.SetString("value", "Text");
	JSON_Object field4 = new JSON_Object();
	field4.SetString("name", "Field 4");
	field4.SetString("value", "Text");
	fields.PushObject(field1);
	fields.PushObject(field2);
	fields.PushObject(field3);
	fields.PushObject(field4);
	embed.SetObject("fields", fields);
	
	embeds.PushObject(embed);
	
	DiscordWebhook_ExecuteJson(id, token, "Test message!", OnDWResponseReceived, .embeds = embeds);
	
	embeds.Cleanup();
	delete embeds;
	
	g_Profiler.Stop();
	PrintToServer("TestJson took %f ms", g_Profiler.Time*1000.0);
}

void TestJsonWithMethodmaps()
{
	g_Profiler.Start();
	
	JSON_Array embeds = new JSON_Array();
	
	embeds.PushObject((new RichEmbed())
	                  .SetTitle("Test!")
	                  .SetDescription("Description")
	                  .SetUrl("https://github.com/haxtonsale")
	                  .SetTimestamp("2020-01-10T18:10:00.000Z")
	                  .SetColor(0x6E14FF)
	                  .SetFooter(new EmbedFooter("Footer text", "https://sun9-55.userapi.com/c543105/v543105788/6190f/vO_rcer0dz8.jpg"))
	                  .SetImage(new EmbedImage("https://sun9-56.userapi.com/c543105/v543105102/6f1ba/HvB5HmGHPCc.jpg"))
	                  .SetThumbnail(new EmbedImage("https://sun1-88.userapi.com/c856524/v856524904/aa1f6/IRHKoE_2_Ng.jpg"))
	                  .SetAuthor(new EmbedAuthor("Haxton Sale", "https://github.com/haxtonsale", "https://avatars2.githubusercontent.com/u/26278199"))
	                  .AddField(new EmbedField("Field 1", "Text", true))
	                  .AddField(new EmbedField("Field 2", "Text", true))
	                  .AddField(new EmbedField("Field 3", "Text"))
	                  .AddField(new EmbedField("Field 4", "Text")));
	
	DiscordWebhook_ExecuteJson(ID5, TOKEN5, "Test message!", OnDWResponseReceived, .embeds = embeds);
	
	embeds.Cleanup();
	delete embeds;
	
	g_Profiler.Stop();
	PrintToServer("TestJsonWithMethodmaps took %f ms", g_Profiler.Time*1000.0);
}

void OnDWResponseReceived(int code, char[] webhookId, DWRateLimits rateLimits)
{
	if (code >= 400)
	{
		char err[256];
		DiscordWebhook_GetErrorMsg(err, sizeof(err));
		LogError("SMDiscordWebhook HTML Error %i: %s", code, err);
	}
	else
	{
		//PrintToServer("(%s): Limit %i; Remaining %i; Reset %i", webhookId, rateLimits.Limit, rateLimits.Remaining, rateLimits.Reset);
	}
}