#if defined _discordwebhook_included
	#endinput
#endif
#define _discordwebhook_included

#define DISCORDWEBHOOK_VERSION "1.0.0"

#tryinclude <json>

#if defined DISCORDWEBHOOK_USE_STEAMWORKS
	#include <SteamWorks>
#elseif defined DISCORDWEBHOOK_USE_STEAMTOOLS
	#include <steamtools>
#else
	#tryinclude <SteamWorks>
	#if !defined _SteamWorks_Included
		#tryinclude <steamtools>
		#if !defined _steamtools_included
			#error "You must have at least one of these libraries to use SMDiscordWebhook: SteamWorks, Steamtools."
		#endif
	#endif
#endif

#if defined _json_included
#include <discordwebhook/richembed>
#endif

enum struct DWRateLimits
{
	int Limit;
	int Remaining;
	int Reset;
}

typedef DiscordWebhook_OnResponse = function void (int code, char[] webhookId, DWRateLimits rateLimits);

static char g_ErrorBuf[256];

// Scummy but works for http error messages
static void GetJsonString(const char[] json, const char[] string, char[] buffer, int bufferlen)
{
	int index = StrContains(json, string);
	if (index > -1)
	{
		index += strlen(string)+4;
		int endIndex = index;
		while (json[endIndex] != '"')
			++endIndex; 
		
		int len = endIndex - index + 1;
		for (int i = 0; i < len && i < bufferlen; i++)
		{
			if (i == len - 1)
				buffer[i] = '\0';
			else
				buffer[i] = json[i + index];
		}
	}
}

static void Callback(int code, DataPack datapack, DWRateLimits rateLimits)
{
	char webhookId[32];
	datapack.Reset();
	datapack.ReadString(webhookId, sizeof(webhookId));
	
	Call_StartFunction(null, datapack.ReadFunction());
	Call_PushCell(code);
	Call_PushString(webhookId);
	Call_PushArray(rateLimits, sizeof(rateLimits));
	Call_Finish();
}

/* 
 * Executes a webhook with Content-Type set to application/x-www-form-urlencoded
 */
stock void DiscordWebhook_Execute(const char[] webhookId, 
                                  const char[] webhookToken,
                                  const char[] content = "",
                                  DiscordWebhook_OnResponse callback = INVALID_FUNCTION,
                                  const char[] username = "",
                                  const char[] avatarUrl = "",
                                  bool tts = false)
{
	char url[128];
	FormatEx(url, sizeof(url), "https://discordapp.com/api/webhooks/%s/%s", webhookId, webhookToken);
	
	DataPack datapack;
	if (callback != INVALID_FUNCTION)
	{
		datapack = new DataPack();
		datapack.WriteString(webhookId);
		datapack.WriteFunction(callback);
	}
	
	#if defined _SteamWorks_Included
	Handle request = SteamWorks_CreateHTTPRequest(k_EHTTPMethodPOST, url);
	SteamWorks_SetHTTPRequestHeaderValue(request, "Content-Type", "application/x-www-form-urlencoded");
	SteamWorks_SetHTTPRequestGetOrPostParameter(request, "content", content);
	
	if (callback != INVALID_FUNCTION)
	{
		SteamWorks_SetHTTPRequestContextValue(request, datapack);
		SteamWorks_SetHTTPCallbacks(request, Webhook_DWRequestCompleted);
	}
	
	if (username[0] != '\0')
		SteamWorks_SetHTTPRequestGetOrPostParameter(request, "username", username);
	
	if (avatarUrl[0] != '\0')
		SteamWorks_SetHTTPRequestGetOrPostParameter(request, "avatar_url", avatarUrl);
	
	if (tts)
		SteamWorks_SetHTTPRequestGetOrPostParameter(request, "tts", "true");
	
	SteamWorks_SendHTTPRequest(request);
	#elseif defined _steamtools_included
	HTTPRequestHandle request = Steam_CreateHTTPRequest(HTTPMethod_POST, url);
	Steam_SetHTTPRequestHeaderValue(request, "Content-Type", "application/x-www-form-urlencoded");
	Steam_SetHTTPRequestGetOrPostParameter(request, "content", content);
	
	if (username[0] != '\0')
		Steam_SetHTTPRequestGetOrPostParameter(request, "username", username);
	
	if (avatarUrl[0] != '\0')
		Steam_SetHTTPRequestGetOrPostParameter(request, "avatar_url", avatarUrl);
	
	if (tts)
		Steam_SetHTTPRequestGetOrPostParameter(request, "tts", "true");
	
	Steam_SendHTTPRequest(request, (callback != INVALID_FUNCTION) ? Webhook_DWRequestCompleted : INVALID_FUNCTION, datapack);
	#endif
}

#if defined _json_included
/* 
 * Executes a webhook with Content-Type set to application/json
 */
stock void DiscordWebhook_ExecuteJson(const char[] webhookId, 
                                      const char[] webhookToken,
                                      const char[] content = "",
                                      DiscordWebhook_OnResponse callback = INVALID_FUNCTION,
                                      const char[] username = "",
                                      const char[] avatarUrl = "",
                                      bool tts = false,
                                      JSON_Array embeds = null)
{
	JSON_Object json = new JSON_Object();
	
	if (content[0] != '\0')
		json.SetString("content", content);
	
	if (username[0] != '\0')
		json.SetString("username", username);
	
	if (username[0] != '\0')
		json.SetString("username", username);
	
	if (avatarUrl[0] != '\0')
		json.SetString("avatar_url", avatarUrl);
	
	if (tts)
		json.SetBool("tts", true);
	
	if (embeds)
		json.SetObject("embeds", embeds);
	
	char body[2048];
	json.Encode(body, sizeof(body));
	delete json;
	
	DiscordWebhook_ExecuteRaw(webhookId, webhookToken, "application/json", body, callback);
}
#endif

stock void DiscordWebhook_ExecuteRaw(const char[] webhookId, 
                                     const char[] webhookToken,
                                     const char[] contentType,
                                     const char[] body,
                                     DiscordWebhook_OnResponse callback = INVALID_FUNCTION)
{
	char url[128];
	FormatEx(url, sizeof(url), "https://discordapp.com/api/webhooks/%s/%s", webhookId, webhookToken);
	
	DataPack datapack;
	if (callback != INVALID_FUNCTION)
	{
		datapack = new DataPack();
		datapack.WriteString(webhookId);
		datapack.WriteFunction(callback);
	}
	
	#if defined _SteamWorks_Included
	Handle request = SteamWorks_CreateHTTPRequest(k_EHTTPMethodPOST, url);
	SteamWorks_SetHTTPRequestRawPostBody(request, contentType, body, strlen(body));
	
	if (callback != INVALID_FUNCTION)
	{
		SteamWorks_SetHTTPRequestContextValue(request, datapack);
		SteamWorks_SetHTTPCallbacks(request, Webhook_DWRequestCompleted);
	}
	
	SteamWorks_SendHTTPRequest(request);
	#elseif defined _steamtools_included
	HTTPRequestHandle request = Steam_CreateHTTPRequest(HTTPMethod_POST, url);
	Steam_SetHTTPRequestRawPostBody(request, body, strlen(body), contentType);
	
	Steam_SendHTTPRequest(request, (callback != INVALID_FUNCTION) ? Webhook_DWRequestCompleted : INVALID_FUNCTION, datapack);
	#endif
}

stock void DiscordWebhook_GetErrorMsg(char[] buffer, int bufferlen)
{
	strcopy(buffer, bufferlen, g_ErrorBuf);
}

#if defined _SteamWorks_Included
public void Webhook_DWRequestCompleted(Handle request, bool fail, bool succ, EHTTPStatusCode code, DataPack datapack)
{
	DWRateLimits rateLimits;
	if (code >= k_EHTTPStatusCode400BadRequest)
	{
		int size;
		SteamWorks_GetHTTPResponseBodySize(request, size);
		
		char[] response = new char[size];
		SteamWorks_GetHTTPResponseBodyData(request, response, size);
		
		GetJsonString(response, "message", g_ErrorBuf, sizeof(g_ErrorBuf));
	}
	else
	{
		char limitstr[16];
		
		SteamWorks_GetHTTPResponseHeaderValue(request, "X-RateLimit-Limit", limitstr, sizeof(limitstr));
		rateLimits.Limit = StringToInt(limitstr);
		
		SteamWorks_GetHTTPResponseHeaderValue(request, "X-RateLimit-Remaining", limitstr, sizeof(limitstr));
		rateLimits.Remaining = StringToInt(limitstr);
		
		SteamWorks_GetHTTPResponseHeaderValue(request, "X-RateLimit-Reset", limitstr, sizeof(limitstr));
		rateLimits.Reset = StringToInt(limitstr);
	}
	
	if (datapack)
	{
		Callback(view_as<int>(code), datapack, rateLimits);
		delete datapack;
	}
	
	delete request;
}
#elseif defined _steamtools_included
public void Webhook_DWRequestCompleted(HTTPRequestHandle request, bool succ, HTTPStatusCode code, DataPack datapack)
{
	DWRateLimits rateLimits;
	if (code >= HTTPStatusCode_BadRequest)
	{
		int size = Steam_GetHTTPResponseBodySize(request);
		
		char[] response = new char[size];
		Steam_GetHTTPResponseBodyData(request, response, size);
		
		GetJsonString(response, "message", g_ErrorBuf, sizeof(g_ErrorBuf));
	}
	else
	{
		char limitstr[16];
		
		Steam_GetHTTPResponseHeaderValue(request, "X-RateLimit-Limit", limitstr, sizeof(limitstr));
		rateLimits.Limit = StringToInt(limitstr);
		
		Steam_GetHTTPResponseHeaderValue(request, "X-RateLimit-Remaining", limitstr, sizeof(limitstr));
		rateLimits.Remaining = StringToInt(limitstr);
		
		Steam_GetHTTPResponseHeaderValue(request, "X-RateLimit-Reset", limitstr, sizeof(limitstr));
		rateLimits.Reset = StringToInt(limitstr);
	}
	
	if (datapack)
	{
		Callback(view_as<int>(code), datapack, rateLimits);
		delete datapack;
	}
	
	Steam_ReleaseHTTPRequest(request);
}
#endif