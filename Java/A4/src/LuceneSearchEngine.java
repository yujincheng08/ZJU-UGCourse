import Crawler.SearchEngine;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
import org.wltea.analyzer.lucene.IKAnalyzer;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class LuceneSearchEngine implements SearchEngine
{
    private File indexFile;
    private Document document;
    private boolean exits = false;
    LuceneSearchEngine(String indexPath)
    {
        indexFile = new File(indexPath);
        exits = indexFile.exists();
        if(!indexFile.exists())
            if(!indexFile.mkdir())
                 throw new RuntimeException("Permission denied");
    }

    @Override
    public void addContext(String url, String title, String content)
    {
        if(document == null)
            document = new Document();
        document.add(new StringField("url",url, Field.Store.YES));
        document.add(new TextField("title", title, Field.Store.YES));
        document.add(new TextField("content", title, Field.Store.YES));
    }

    @Override
    public void search()
    {
        Analyzer analyzer = new IKAnalyzer();
        try
        {
            if(document != null)
            {
                IndexWriter indexWriter = new IndexWriter(FSDirectory.open(indexFile), new IndexWriterConfig(Version.LUCENE_4_10_0, analyzer));
                indexWriter.addDocument(document);
                indexWriter.close();
            }
            IndexSearcher indexSearcher = new IndexSearcher(DirectoryReader.open(FSDirectory.open(indexFile)));
            QueryParser titleQueryParser = new QueryParser("title", analyzer);
            QueryParser contentQueryParser = new QueryParser("content", analyzer);
            ScoreDoc[] docs = null;
            boolean titleFinished = false;
            System.out.print("Please enter searching keywords: ");
            Scanner in = new Scanner(System.in);
            String queryString = in.next();
            Mainloop: while(true) {
                if(docs == null)
                    docs = indexSearcher.search(titleQueryParser.parse(queryString), 10).scoreDocs;
                else if(!titleFinished)
                    docs = indexSearcher.searchAfter(docs[docs.length-1], contentQueryParser.parse(queryString), 10).scoreDocs;
                else
                    docs = indexSearcher.searchAfter(docs[docs.length-1], titleQueryParser.parse(queryString), 10).scoreDocs;
                for (ScoreDoc doc : docs)
                    outputDoc(indexSearcher, doc);
                if(docs.length < 10) {
                    if (titleFinished) {
                        System.out.println("Searching ends.");
                        break;
                    }
                    else {
                        titleFinished = true;
                        docs = indexSearcher.search(titleQueryParser.parse(queryString), 10 - docs.length).scoreDocs;
                        for (ScoreDoc doc : docs)
                            outputDoc(indexSearcher, doc);
                    }
                }
                while(true) {
                    System.out.println("Input `next` to the next page, input `exit` to end searching.");
                    String inst = in.next();
                    switch(inst)
                    {
                        case "next": continue Mainloop;
                        case "exit": break Mainloop;
                        default: System.out.println("Unknown input. Please try again.");
                    }
                }
            }
        } catch (IOException | ParseException e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public boolean exists() {
        return exits;
    }

    private void outputDoc(IndexSearcher indexSearcher, ScoreDoc doc)
            throws IOException
    {
        Document d = indexSearcher.doc(doc.doc);
        String url = d.get("url");
        String title = d.get("title");
        String content = d.get("content");
        System.out.println(title);
        System.out.println(url);
        System.out.println(content);
        System.out.println("----------------");
    }
}
