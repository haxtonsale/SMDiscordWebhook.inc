# SMDiscordWebhook.inc
.inc that sends messages and embeds through discord webhooks in SourceMod for you.
## Requirements
Requires [SteamWorks](https://forums.alliedmods.net/showthread.php?t=229556) or [SteamTools](https://builds.limetech.io/?p=steamtools).<br/>[sm-json](https://github.com/clugg/sm-json) is required for `DiscordWebhook_ExecuteJson`.
## Sending messages
```c
#include <sourcemod>
#include <SteamWorks>
#include <discordwebhook>

public int SteamWorks_SteamServersConnected()
{
    DiscordWebhook_Execute(WebhookId, WebhookToken, "Test message!", OnDWResponseReceived);
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
        PrintToServer("(%s): Limit %i; Remaining %i; Reset %i", webhookId, rateLimits.Limit, rateLimits.Remaining, rateLimits.Reset);
    }
}
```
## Sending a JSON body using DiscordWebhook_ExecuteRaw
```c
#include <sourcemod>
#include <SteamWorks>
#include <discordwebhook>

public int SteamWorks_SteamServersConnected()
{
    char body[] = "{\"content\": \"Test message!\", \"embeds\": [{\"title\": \"Embed\"}]}";
    DiscordWebhook_ExecuteRaw(WebhookId, WebhookToken, "application/json", body);
}
```
## Sending a message using DiscordWebhook_ExecuteJson (Requires sm-json)
```c
#include <sourcemod>
#include <SteamWorks>
#include <json>
#include <discordwebhook>

public int SteamWorks_SteamServersConnected()
{
    RichEmbed embed = new RichEmbed();
    embed.SetTitle("Embed Title");
    embed.SetColor(0x6E14FF);
    embed.SetFooter(new EmbedFooter("Footer"));
    embed.SetImage(new EmbedImage("https://sun9-56.userapi.com/c543105/v543105102/6f1ba/HvB5HmGHPCc.jpg"));
    embed.SetAuthor(new EmbedAuthor("Author"));
    embed.AddField(new EmbedField("Field 1", "Text", true));
    embed.AddField(new EmbedField("Field 2", "More text", true));
    
    JSON_Array embeds = new JSON_Array();
    embeds.PushObject(embed);
    
    DiscordWebhook_ExecuteJson(WebhookId, WebhookToken, "Test message!", .embeds = embeds);
    
    embeds.Cleanup();
    delete embeds;
}
```
