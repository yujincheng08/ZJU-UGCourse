import Crawler.SearchEngine;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.queryparser.classic.MultiFieldQueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.highlight.*;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

import org.wltea.analyzer.lucene.IKAnalyzer;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class LuceneSearchEngine implements SearchEngine
{
    private static final int FRAGMENT_SIZE = 150;
    private File indexFile;
    private boolean exits;
    private IndexWriter indexWriter;
    private Analyzer analyzer = new IKAnalyzer(true);
    private static final Formatter formatter = new SimpleHTMLFormatter("【【", "】】");
    LuceneSearchEngine(String indexPath)
    {
        indexFile = new File(indexPath);
        exits = indexFile.exists();
        if(!indexFile.exists())
            if(!indexFile.mkdir())
                 throw new RuntimeException("Permission denied");
        if(exits) {
            try {
                exits = DirectoryReader.indexExists(FSDirectory.open(indexFile));
            }catch(IOException e)
            {
                e.printStackTrace();
            }
        }
        try {
            indexWriter = new IndexWriter(FSDirectory.open(indexFile), new IndexWriterConfig(Version.LUCENE_4_10_0, analyzer));
        }catch(IOException e)
        {
            e.printStackTrace();
        }
    }
@Override
    public void addContext(String url, String title, String content)
    {
        Document document = new Document();
        document.add(new StringField("url",url, Field.Store.YES));
        document.add(new TextField("title", title, Field.Store.YES));
        document.add(new TextField("content", content, Field.Store.YES));
        try {
            if(!exits)
                indexWriter.addDocument(document);
        }catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void search()
    {
        try
        {
            int result = indexWriter.numDocs();
            System.out.println("Indexed "+ result + " files. To re-index, you can manually delete index folder.");
            indexWriter.close();
            //IndexSearcher indexSearcher = new IndexSearcher(DirectoryReader.open(FSDirectory.open(indexFile)));
            //QueryParser titleQueryParser = new QueryParser("title", analyzer);
            ///QueryParser contentQueryParser = new QueryParser("content", analyzer);
            ScoreDoc[] docs;
            Scanner in = new Scanner(System.in);
            IndexSearcher indexSearcher = new IndexSearcher(DirectoryReader.open(FSDirectory.open(indexFile)));
            MultiFieldQueryParser queryParser = new MultiFieldQueryParser(new String[] {"title", "content"}, analyzer);
            queryParser.setDefaultOperator(QueryParser.Operator.AND);
            LuceneSearchPage page = new LuceneSearchPage();
            while(true) {
                page.clear();
                System.out.print("Please enter searching keywords(`quit` to quit): ");
                String queryString = in.next();
                if(queryString.equals("quit"))
                    break;
                Query query = queryParser.parse(queryString);
                docs = indexSearcher.search(query, 10).scoreDocs;
                while (docs.length > 0) {
                    for (ScoreDoc doc : docs)
                        addDocToPage(page, indexSearcher, doc, query);
                    docs = indexSearcher.searchAfter(docs[docs.length - 1], query, 10).scoreDocs;
                }
                page.execPage();
            }
        } catch (IOException | ParseException e)
        {
            e.printStackTrace();
        }
        System.out.println("Thank you!");
    }

    @Override
    public boolean exists() {
        return exits;
    }

    private void addDocToPage(LuceneSearchPage page, IndexSearcher indexSearcher, ScoreDoc doc, Query query)
            throws IOException
    {
        Highlighter highlighter = new Highlighter(formatter, new QueryScorer(query));
        highlighter.setTextFragmenter(new SimpleFragmenter(FRAGMENT_SIZE));
        Document d = indexSearcher.doc(doc.doc);
        String url = d.get("url");
        String title = d.get("title");
        String content = d.get("content");
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(url).append("\n");
        try {
            TokenStream titleTokenStream = analyzer.tokenStream("", title);
            String titleFragment = highlighter.getBestFragment(titleTokenStream, title);
            if(titleFragment != null) titleFragment = titleFragment.replaceAll("】】【【", "");
            TokenStream contentTokenStream = analyzer.tokenStream("", content);
            String contentFragment = highlighter.getBestFragment(contentTokenStream, content);
            if(contentFragment != null) contentFragment = contentFragment.replaceAll("】】【【","");
            if(titleFragment == null || titleFragment.isEmpty())
                titleFragment = title;
            if(contentFragment == null || contentFragment.isEmpty()) {
                if (content.length() > FRAGMENT_SIZE)
                    contentFragment = content.substring(0, FRAGMENT_SIZE);
                else contentFragment = content;
            }
            stringBuilder.append(titleFragment).append("\n").append(contentFragment).append("\n--------------\n");
            page.addRecord(stringBuilder.toString());
        }catch (InvalidTokenOffsetsException e)
        {
            e.printStackTrace();
        }
    }
}
