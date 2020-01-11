methodmap EmbedFooter < Handle
{
	public bool SetText(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("text", value);
	}
	
	public bool SetIconUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("icon_url", value);
	}
	
	public bool SetProxyIconUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("proxy_icon_url", value);
	}
	
	public EmbedFooter(const char[] text, const char[] iconUrl = "", const char[] proxyIconUrl = "")
	{
		EmbedFooter self = view_as<EmbedFooter>(new JSON_Object());
		view_as<JSON_Object>(self).Clear();
		
		self.SetText(text);
		if (iconUrl[0] != '\0')
			self.SetIconUrl(iconUrl);
		if (proxyIconUrl[0] != '\0')
			self.SetProxyIconUrl(proxyIconUrl);
		
		return self;
	}
}

methodmap EmbedImage < Handle
{
	public bool SetUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("url", value);
	}
	
	public bool SetProxyUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("proxy_url", value);
	}
	
	public EmbedImage(const char[] url = "", const char[] proxyUrl = "")
	{
		EmbedImage self = view_as<EmbedImage>(new JSON_Object());
		view_as<JSON_Object>(self).Clear();
		
		if (url[0] != '\0')
			self.SetUrl(url);
		if (proxyUrl[0] != '\0')
			self.SetProxyUrl(proxyUrl);
		
		return self;
	}
}

methodmap EmbedAuthor < Handle
{
	public bool SetName(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("name", value);
	}
	
	public bool SetUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("url", value);
	}
	
	public bool SetIconUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("icon_url", value);
	}
	
	public bool SetProxyIconUrl(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("proxy_icon_url", value);
	}
	
	public EmbedAuthor(const char[] name = "", const char[] url = "", const char[] iconUrl = "", const char[] proxyIconUrl = "")
	{
		EmbedAuthor self = view_as<EmbedAuthor>(new JSON_Object());
		view_as<JSON_Object>(self).Clear();
		
		if (name[0] != '\0')
			self.SetName(name);
		if (url[0] != '\0')
			self.SetUrl(url);
		if (iconUrl[0] != '\0')
			self.SetIconUrl(iconUrl);
		if (proxyIconUrl[0] != '\0')
			self.SetProxyIconUrl(proxyIconUrl);
		
		return self;
	}
}

methodmap EmbedField < Handle
{
	public bool SetName(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("name", value);
	}
	
	public bool SetValue(const char[] value)
	{
		return view_as<JSON_Object>(this).SetString("value", value);
	}
	
	public bool SetInline(bool value)
	{
		return view_as<JSON_Object>(this).SetBool("inline", value);
	}
	
	public EmbedField(const char[] name, const char[] value, bool inline = false)
	{
		EmbedField self = view_as<EmbedField>(new JSON_Object());
		view_as<JSON_Object>(self).Clear();
		
		self.SetName(name);
		self.SetValue(value);
		if (inline)
			self.SetInline(true);
		
		return self;
	}
}

methodmap RichEmbed < JSON_Object
{
	public RichEmbed SetTitle(const char[] value)
	{
		this.SetString("title", value);
		return this;
	}
	
	public RichEmbed SetDescription(const char[] value)
	{
		this.SetString("description", value);
		return this;
	}
	
	public RichEmbed SetUrl(const char[] value)
	{
		this.SetString("url", value);
		return this;
	}
	
	public RichEmbed SetTimestamp(const char[] value)
	{
		this.SetString("timestamp", value);
		return this;
	}
	
	public RichEmbed SetColor(int value)
	{
		this.SetInt("color", value);
		return this;
	}
	
	public RichEmbed SetFooterObj(EmbedFooter footer)
	{
		this.SetObject("footer", view_as<JSON_Object>(footer));
		return this;
	}
	public RichEmbed SetFooter(const char[] text, const char[] iconUrl = "", const char[] proxyIconUrl = "")
	{
		return this.SetFooterObj(new EmbedFooter(text, iconUrl, proxyIconUrl));
	}
	
	public RichEmbed SetImageObj(EmbedImage image)
	{
		this.SetObject("image", view_as<JSON_Object>(image));
		return this;
	}
	public RichEmbed SetImage(const char[] url = "", const char[] proxyUrl = "")
	{
		return this.SetImageObj(new EmbedImage(url, proxyUrl));
	}
	
	public RichEmbed SetThumbnailObj(EmbedImage thumbnail)
	{
		this.SetObject("thumbnail", view_as<JSON_Object>(thumbnail));
		return this;
	}
	public RichEmbed SetThumbnail(const char[] url = "", const char[] proxyUrl = "")
	{
		return this.SetThumbnailObj(new EmbedImage(url, proxyUrl));
	}
	
	public RichEmbed SetAuthorObj(EmbedAuthor author)
	{
		this.SetObject("author", view_as<JSON_Object>(author));
		return this;
	}
	public RichEmbed SetAuthor(const char[] name = "", const char[] url = "", const char[] iconUrl = "", const char[] proxyIconUrl = "")
	{
		return this.SetAuthorObj(new EmbedAuthor(name, url, iconUrl, proxyIconUrl));
	}
	
	public RichEmbed AddFieldObj(EmbedField field)
	{
		if (!this.HasKey("fields"))
			this.SetObject("fields", new JSON_Array());
		
		view_as<JSON_Array>(this.GetObject("fields")).PushObject(view_as<JSON_Object>(field));
		return this;
	}
	public RichEmbed AddField(const char[] name, const char[] value, bool inline = false)
	{
		return this.AddFieldObj(new EmbedField(name, value, inline));
	}
	
	public RichEmbed()
	{
		RichEmbed self = view_as<RichEmbed>(new JSON_Object());
		self.Clear();
		
		return self;
	}
}