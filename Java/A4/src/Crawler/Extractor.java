package Crawler;

public interface Extractor
{
    void extract(String url, String html);
    String getTitle();
    String getContent();
}
