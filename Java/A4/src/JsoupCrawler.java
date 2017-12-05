import Crawler.Crawler;
import org.jsoup.HttpStatusException;
import org.jsoup.Jsoup;
import org.jsoup.UnsupportedMimeTypeException;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.net.ConnectException;

public class JsoupCrawler extends Crawler
{
    private static final int MAXTRY = 3;
    @Override
    protected String handleUrl(String url) {
        for (int retry = 1; retry <= MAXTRY; retry++) {
            try {
                Document doc;
                doc = Jsoup.connect(url).timeout(3000).maxBodySize(10*1024*1024).get();
                Elements as = doc.select("a[href]");
                for (Element a : as) {
                    String href = a.attr("abs:href");
                    addUrl(href);
                }
                outputStatus(url, "Success.");
                return doc.html();
            } catch (UnsupportedMimeTypeException e) {
                outputStatus(url, "Unsupported type.");
                return null;
            } catch (HttpStatusException e) {
                outputStatus(url,"Http " + e.getStatusCode() + " error.");
                return null;
            } catch (ConnectException e) {
                outputStatus(url,"Connection time out. Retrying for " + retry + " time(s)...");
            } catch (Throwable e) {
                outputStatus(url, e.getMessage() + ". Retrying for " + retry + " time(s)...");
            }
        }
        return null;
    }
}
