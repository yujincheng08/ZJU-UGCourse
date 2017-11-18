import Crawler.Crawler;
import Crawler.SearchEngine;
import org.jsoup.HttpStatusException;
import org.jsoup.Jsoup;
import org.jsoup.UnsupportedMimeTypeException;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.IOException;
import java.io.UncheckedIOException;
import java.net.ConnectException;

public class JsoupCrawler extends Crawler
{

    @Override
    protected String handleUrl(String url)
    {
        try
        {
            Document doc;
            doc = Jsoup.connect(url).get();
            Elements as = doc.select("a[href]");
            for (Element a : as)
            {
                String href = a.attr("abs:href");
                addUrl(href);
            }
            return doc.html();
        } catch (UnsupportedMimeTypeException e)
        {
            handleStatus("Unsupported type.");
        } catch (HttpStatusException e)
        {
            handleStatus("Http " + e.getStatusCode() + " error.");
        } catch (ConnectException e)
        {
            handleStatus("Connection time out.");
        } catch (Exception e)
        {
            handleStatus(e.getMessage());
        }
        return null;
    }
}
