import org.apache.lucene.document.Document;

import java.util.ArrayList;
import java.util.Scanner;

public class LuceneSearchPage {
    private static int PAGE_SIZE = 5;
    private int pageLength = PAGE_SIZE;
    private int start = 0;
    private ArrayList<String> records = new ArrayList<>();

    public LuceneSearchPage() {
        this(5);
    }

    public LuceneSearchPage(int pageLength)
    {
        this.pageLength = pageLength;
    }

    public int getRecordSize()
    {
        return records.size();
    }

    public int getPageLength()
    {
        return pageLength;
    }

    public int getPageSize()
    {
        int pageLength = getPageLength();
        int recordSize = getRecordSize();
        return recordSize % pageLength == 0 ? recordSize / pageLength : recordSize / pageLength + 1;
    }

    public int getCurrentPageNo()
    {
        return start / getPageLength() + 1;
    }

    public boolean hasNextPage()
    {
        return getCurrentPageNo() < getPageSize();
    }

    public boolean hasPreviousPage()
    {
        return getCurrentPageNo() > 1;
    }

    public void addRecord(String record)
    {
        records.add(record);
    }

    public void clear()
    {
        records.clear();
    }

    public void nextPage()
    {
        if(hasNextPage())
            start += pageLength;
    }

    public void previousPage()
    {
        if(hasPreviousPage())
            start -= pageLength;
    }

    public String getPage()
    {
        StringBuilder stringBuilder = new StringBuilder();
        for(int i = start; i < start + pageLength && i < getRecordSize(); ++i)
            stringBuilder.append(records.get(i));
        return stringBuilder.toString();
    }

    public void execPage()
    {
        Scanner in = new Scanner(System.in);
        SearchLoop: while(true)
        {
            System.out.println(getPage());
            System.out.printf("Current Page: %d\tTotal Page: %d\tTotal Records:%d\n", getCurrentPageNo(),
                    getPageSize(), getRecordSize());
            while(true)
            {
                System.out.print("`next` or `prev` or `exit`: ");
                switch(in.next()) {
                    case "next":
                        nextPage();
                        continue SearchLoop;
                    case "prev":
                        previousPage();
                        continue SearchLoop;
                    case "exit":
                        break SearchLoop;
                    default:
                        System.out.println("Unexpected token.");
                }
            }
        }
    }


}
