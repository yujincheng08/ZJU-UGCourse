package Crawler;

public interface SearchEngine
{
    void addContext(String url, String title, String content);
    void search();
    boolean exists();
}
