package Crawler;

import java.util.HashSet;
import java.util.LinkedList;

public class UrlSet
{
    private HashSet<String> tracked = new HashSet<>();
    private LinkedList<String> untracked = new LinkedList<>();

    private static String deleteHash(String url)
    {
        int endPos = url.lastIndexOf('#');
        if(endPos > 0)
            return url.substring(0, endPos);
        else
            return url;
    }

    boolean addUrl(String url)
    {
        url = deleteHash(url);
        if(!tracked.contains(url))
        {
            tracked.add(url);
            untracked.offer(url);
            return true;
        }
        return false;
    }

    String getUrl()
    {
        if(untracked.size()>0)
            return untracked.poll();
        else
            return null;
    }

    boolean isEmpty()
    {
        return untracked.isEmpty();
    }
}
