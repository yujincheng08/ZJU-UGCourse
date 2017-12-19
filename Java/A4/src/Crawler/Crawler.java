package Crawler;

import java.util.concurrent.Semaphore;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;

public abstract class Crawler
{
    private static int THREAD_SIZE = 16;
    private SearchEngine searchEngine;
    private String filter = ".*";
    private Extractor extractor;
    private UrlSet urlSet = new UrlSet();
    private BlockThreadPoolExecutor executor = new BlockThreadPoolExecutor(THREAD_SIZE, 60L, TimeUnit.MILLISECONDS);
    private Semaphore semaphore = new Semaphore(-1);
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
        if(url != null && matchesFilter(url) && urlSet.addUrl(url)) {
            semaphore.release();
        }

    }

    private String getUrl()
    {
        return  urlSet.getUrl();
    }

    private synchronized void extract(String url, String content)
    {
        extractor.extract(url, content);
        addContext(url, extractor.getTitle(), extractor.getContent());
    }

    private synchronized void addContext(String url, String title, String content)
    {
        searchEngine.addContext(url, title, content);
    }

    protected synchronized void outputStatus(String url, String status)
    {
        System.out.println("[" + urlSet.size() + "] " + url + ": " + status);
    }

    public void crawl(String startUrl)
    {
        if(searchEngine.exists())
            return;
        checkDependencies();
        addUrl(startUrl);
        while(true)
        {
            final String url = getUrl();
            try {
                executor.blockSubmit(() -> {
                    String content = handleUrl(url);
                    if (content != null) {
                        extract(url, content);
                    }
                });
                if(urlSet.isEmpty()){
                    executor.await();
                    if(urlSet.isEmpty())
                        break;
                    else
                        continue;
                }
                semaphore.acquire();
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        System.out.println("Ends crawling.");
        executor.shutdown();
    }

    public void search()
    {
        searchEngine.search();
    }

    protected abstract String handleUrl(String url);
}
