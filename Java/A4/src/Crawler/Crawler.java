package Crawler;
public abstract class Crawler
{
    private SearchEngine searchEngine;
    private String filter = ".*";
    private Extractor extractor;
    private UrlSet urlSet = new UrlSet();

    public Crawler setSearchEngine(SearchEngine searchEngine)
    {
        this.searchEngine = searchEngine;
        return this;
    }

    public Crawler setFilter(String regex)
    {
        this.filter = regex;
        return this;
    }

    public Crawler setExtractor(Extractor extractor)
    {
        this.extractor = extractor;
        return this;
    }

    protected boolean matchesFilter(String url)
    {
        if (filter == null) throw new RuntimeException("Filter not set!");
        return url.matches(filter);
    }

    private void checkDependencies()
    {
        if (extractor == null) throw new RuntimeException("Extractor not set");
        if (searchEngine == null ) throw new RuntimeException("Search engine not set");
    }

    protected void addUrl(String url)
    {
        if(matchesFilter(url))
            urlSet.addUrl(url);

    }

    protected String getUrl()
    {
        return urlSet.getUrl();
    }

    protected void handleStatus(String status)
    {
        System.out.print(status);
    }

    public void crawl(String url)
    {
        checkDependencies();
        addUrl(url);
        while(!urlSet.isEmpty())
        {
            url = getUrl();
            System.out.print("Crawling " + url + ": ");
            String content = handleUrl(url);
            System.out.println();
            if(content == null)
                continue;
            extractor.extract(url, content);
            System.out.println("Title: " + extractor.getTitle());
            System.out.println("Content: " + extractor.getContent());
        }
    }

    protected abstract String handleUrl(String url);
}
