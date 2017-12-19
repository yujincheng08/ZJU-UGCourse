import Crawler.Crawler;
import Crawler.SearchEngine;
import Crawler.Extractor;
import org.jsoup.nodes.Document;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

class Main
{
    public static void main(String [] arg) throws Exception
    {
        Crawler crawler = new JsoupCrawler()
                .setFilter(".+://(www\\.)?(cs|cspo)\\.zju\\.edu\\.cn/((?!catalog_id=23471&).)*")
                .setSearchEngine(new LuceneSearchEngine("./index"))
                .setExtractor(new Extractor()
                {
                    String content;
                    String title;

                    private String textWithBreaks(Elements eles)
                    {
                        eles.select("br").append("\\n");
                        eles.select("p").append("\\n\\n");
                        return eles.text().replaceAll("\\\\n","\n").trim();
                    }

                    @Override
                    public void extract(String url, String html)
                    {
                        Document doc = Jsoup.parse(html);
                        title = doc.title();
                        content = textWithBreaks(doc.select("p, font, li, td, tr"));
                        if (url.matches("http://cspo\\.zju\\.edu\\.cn/cspo_bks/.*"))
                        {
                            Elements right = doc.select(".right");
                            Elements links = doc.select("*[align=right]");
                            for(Element a : links)
                                a.remove();
                            if(right.size()>0)
                            {
                                Elements t = right.select("center font");
                                if(t.size()>0)
                                    title = t.get(0).text();
                                content = textWithBreaks(right);
                            }
                        }
                        else if(url.matches("http://(www\\.cs|cspo)\\.zju\\.edu\\.cn/chinese/.*"))
                        {
                            Elements t = doc.select(".article_title, .right_title > a:last-child");
                            if(t.size()>0)
                                title = t.text();
                            Elements article = doc.select(".article, .article_content");
                            Elements share = article.select(".share-btn");
                            for(Element e : share)
                                e.remove();
                            if(article.size()>0)
                                content = textWithBreaks(article);
                        }
                        else if(url.matches("http://cspo\\.zju\\.edu\\.cn/.*"))
                        {
                            Elements containers = doc.select(".container");
                            if (containers.size() > 2)
                            {
                                Element container = containers.get(1);
                                Elements t = container.select("h1");
                                if (t.size() > 0) title = t.get(0).text();
                                else
                                {
                                    t = container.select(".yxbs1 a:last-child");
                                    if(t.size()>0) title = t.get(0).text();
                                }
                                Elements useless = container.select(".header-yz, .yxbs1");
                                for(Element e : useless)
                                    e.remove();
                                Elements m = container.select("tr:nth-child(3)");
                                if (m.size() > 0) content = textWithBreaks(m);
                                else content = textWithBreaks(new Elements(container));
                            }
                        }
                        else if(url.matches("http://www\\.cs\\.zju\\.edu\\.cn/english/.*"))
                        {
                            Elements middle = doc.select("#main .middle2");
                            if(middle.size()>0)
                            {
                                Elements trs = middle.select("tr");
                                if(trs.size()==5)
                                {
                                    title = trs.get(1).text();
                                    content = textWithBreaks(new Elements(trs.get(3)));
                                }
                                else
                                {
                                    Elements customize = doc.select(".customize");
                                    if(customize.size() > 0)
                                    {
                                        Elements ctrs = customize.select("tr");
                                        if(ctrs.size() == 2)
                                        {
                                            title = ctrs.get(0).text();
                                            content = textWithBreaks(new Elements(ctrs.get(1)));
                                        }
                                        else
                                        {
                                            Elements h = customize.select("h, h1, h2, h3");
                                            if(h.size()>0)
                                                title = h.text();
                                            else
                                                title = doc.title();
                                            Elements p = customize.select("p");
                                            if(p.size()>0)
                                                content = textWithBreaks(p);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    @Override
                    public String getContent()
                    {
                        return content;
                    }

                    public String getTitle()
                    {
                        return title;
                    }
                });
        crawler.crawl("http://www.cs.zju.edu.cn/");
        //crawler.crawl("http://cspo.zju.edu.cn/redir.php?catalog_id=13&object_id=142666");
        crawler.search();
        // crawler.crawl("http://www.cs.zju.edu.cn/chinese/");
        // crawler.crawl("http://cspo.zju.edu.cn/redir.php?catalog_id=23382&object_id=701536");
    }
}



//
//import org.apache.lucene.analysis.Analyzer;
//import org.apache.lucene.anal
//import org.apache.lucene.document.Document;
//import org.apache.lucene.document.Field;
//import org.apache.lucene.index.CorruptIndexException;
//import org.apache.lucene.index.IndexReader;
//import org.apache.lucene.index.IndexWriter;
//import org.apache.lucene.queryParser.ParseException;
//import org.apache.lucene.queryParser.QueryParser;
//import org.apache.lucene.search.IndexSearcher;
//import org.apache.lucene.search.Query;
//import org.apache.lucene.search.ScoreDoc;
//import org.apache.lucene.search.Searcher;
//import org.apache.lucene.search.TopScoreDocCollector;
//import org.apache.lucene.store.FSDirectory;
//import org.apache.lucene.util.Version;
//import org.htmlparser.NodeFilter;
//import org.htmlparser.Parser;
//import org.htmlparser.beans.StringBean;
//import org.htmlparser.filters.NodeClassFilter;
//import org.htmlparser.tags.LinkTag;
//import org.htmlparser.util.NodeList;
//import org.htmlparser.util.ParserException;
//import org.htmlparser.visitors.HtmlPage;
//
//public class ParseURL {
//    //索引目录
//    private static final String INDEX_DIR = "myindex";
//    //已经存在的url列表
//    private static List<String> urls=new ArrayList<String>();
//
//    /**
//     * 索引器，对目标url创建索引
//     * @param url   目标网址
//     * @throws IOException
//     * @throws ParserException
//     */
//    @SuppressWarnings("deprecation")
//    private static void indexer(String url) throws IOException, ParserException {
//        //判断是新创建索引文件，还是附加方式
//        boolean create=false;
//        //存储索引的目录
//        File indexDir = new File(INDEX_DIR);
//        //目录不存在，创建该目录
//        if (!indexDir.exists()) {
//            indexDir.mkdir();
//        }
//        //判断是否需要新创建索引文件
//        if(indexDir.list().length<=0)
//        {
//            create=true;
//        }
//        //获取网页纯文本
//        String content = getText(url);
//        //获取网页标题
//        String title = getTitle(url);
//
//        System.out.println("title:" + title);
//
//        if(title==null || content==null || content.trim().equals(""))
//        {
//            return;
//        }
////      System.out.println("content:" + content);
////      URL path=new URL(url);
////      InputStream stream=path.openStream();
////
////      Reader reader=new InputStreamReader(stream);
//
////      Reader reader=new InputStreamReader(new ByteArrayInputStream(content.getBytes()));
////      Reader reader2=new InputStreamReader(new ByteArrayInputStream(title.getBytes()));
//
//        Document doc = new Document();
//        //加入url域
//        doc.add(new Field("url", url, Field.Store.YES,
//                Field.Index.UN_TOKENIZED));
//        //加入标题域
//        doc.add(new Field("title", title, Field.Store.YES,
//                Field.Index.ANALYZED));
////      doc.add(new Field("title",reader2));
//        //Index.ANALYZED分词后构建索引
//        //加入内容域
//        doc.add(new Field("content", content, Field.Store.YES,
//                Field.Index.ANALYZED));
////      doc.add(new Field("content",reader));
//        //根据索引目录和create的状态创建索引写磁盘对象
//        IndexWriter writer = new IndexWriter(FSDirectory.open(indexDir),
//                new StandardAnalyzer(Version.LUCENE_CURRENT), create,
//                IndexWriter.MaxFieldLength.LIMITED);
//
//        writer.addDocument(doc);
//
//        writer.optimize();
//        writer.close();
//        //创建了索引的网址加入到已经存在的网址列表中
//        urls.add(url);
//    }
//
//    /**
//     * 搜索器，根据输入的文本去搜索
//     * @param words     输入的文本
//     * @throws CorruptIndexException
//     * @throws IOException
//     * @throws ParseException
//     */
//    @SuppressWarnings("deprecation")
//    private static void searcher(String words) throws CorruptIndexException,
//            IOException, ParseException {
//        File indexDir = new File(INDEX_DIR);
//        //根据索引目录创建读索引对象
//        IndexReader reader = IndexReader.open(FSDirectory.open(indexDir), true);
//        //搜索对象创建
//        Searcher searcher = new IndexSearcher(reader);
//        //分词
//        Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_CURRENT);
//        //指定搜索的域
//        String field="content";
//        //创建查询解析对象
//        QueryParser parser = new QueryParser(field, analyzer);
//        //根据域和目标搜索文本创建查询器
//        Query query = parser.parse(words);
//        System.out.println("Searching for: " + query.toString(field));
//        //对结果进行打分排序
//        TopScoreDocCollector collector = TopScoreDocCollector.create(5 * 10,false);
//        searcher.search(query, collector);
//        //获取结果
//        ScoreDoc[] hits = collector.topDocs().scoreDocs;
//
//        int numTotalHits = collector.getTotalHits();
//
//        System.out.println(numTotalHits + " total matching documents");
//        //显示搜索结果
//        for (int i = 0; i < hits.length; i++) {
//            Document doc = searcher.doc(hits[i].doc);
//            String url = doc.get("url");
//            String title=doc.get("title");
//            String content=doc.get("content");
//
//            System.out.println((i + 1) + "." + title);
//            System.out.println("-----------------------------------");
//            System.out.println(content.substring(0,100)+"......");
//            System.out.println("-----------------------------------");
//            System.out.println(url);
//            System.out.println();
//        }
//    }
//
//    /**
//     * 收入网站
//     * @param url   网站首页url，也可以为网站地图url
//     * @throws ParserException
//     * @throws IOException
//     * @throws ParseException
//     */
//    private static void addSite(String url) throws ParserException, IOException, ParseException
//    {
//        long start=System.currentTimeMillis();
//        System.out.println("start add...");
//        //获取目标网页的所有链接
//        List<String> links = getLinks(url);
//        System.out.println("url count:"+links.size());
//        for(int i=0;i<links.size();i++)
//        {
//            String link=links.get(i);
//            System.out.println((i+1)+"."+link);
//
//            if(!urls.contains(link))
//            {
//                //对未创建过索引的网页创建索引
//                indexer(link);
//            }
//            else
//            {
//                System.out.println("["+link+"] exist");
//            }
//        }
//
//        System.out.println("end...");
//
//        long end=System.currentTimeMillis();
//
//        System.out.println("cost "+(end-start)/1000+" seconds");
//    }
//    /**
//     * 获取网页纯文本
//     * @param url   目标网址
//     * @return
//     * @throws ParserException
//     */
//    private static String getText(String url) throws ParserException {
//        StringBean sb = new StringBean();
//
//        // 设置不需要得到页面所包含的链接信息
//        sb.setLinks(false);
//        // 设置将不间断空格由正规空格所替代
//        sb.setReplaceNonBreakingSpaces(true);
//        // 设置将一序列空格由一个单一空格所代替
//        sb.setCollapse(true);
//        // 传入要解析的URL
//        sb.setURL(url);
//        // 返回解析后的网页纯文本信息
//        String content = sb.getStrings();
//        // System.out.println(content);
//        return content;
//    }
//
//    /**
//     * 获取网页标题
//     * @param path
//     * @return
//     * @throws IOException
//     * @throws ParserException
//     */
//    private static String getTitle(String path) throws IOException,
//            ParserException {
//        String title = "";
//
//
//        try {
//
//            Parser parser=new Parser(path);
//
//            HtmlPage page = new HtmlPage(parser);
//            parser.visitAllNodesWith(page);
//            title=page.getTitle();
//
////          URL url = new URL(path);
////          URLConnection conn = url.openConnection();
////          conn.setConnectTimeout(1000*5);
////
////          InputStream stream = conn.getInputStream();
////
////          byte[] bs = new byte[stream.available()];
////          stream.read(bs);
////          String str = new String(bs, "utf-8");
////
////          List<String> list = new ArrayList<String>();
////          Pattern pa = Pattern.compile("<title>.*?</title>");// 源码中标题正则表达式
////          Matcher ma = pa.matcher(str);
////          while (ma.find())// 寻找符合el的字串
////          {
////              list.add(ma.group());// 将符合el的字串加入到list中
////          }
////          for (int i = 0; i < list.size(); i++) {
////              title = title + list.get(i);
////          }
////          title = title.replaceAll("<.*?>", "");
//        } catch (Exception e) {
//            // TODO Auto-generated catch block
//            //e.printStackTrace();
//            title = "no title";
//        }
//
//        return title.trim();
//    }
//
//    /**
//     * 获取网页中所有的链接
//     * @param url
//     * @return
//     * @throws ParserException
//     */
//    private static List<String> getLinks(String url) throws ParserException
//    {
//        List<String> links=new ArrayList<String>();
//        //创建链接节点的过滤器
//        NodeFilter filter = new NodeClassFilter(LinkTag.class);
//        Parser parser = new Parser();
//        parser.setURL(url);
//        //设置目标网页的编码方式
//        //parser.setEncoding("utf-8");
//        //因为有些时候不清楚目标网页的编码方式，这里我们采用指定一
//        //个编码集合，然后通过试探的方式得到目标网页的编码方式
//        parser.setEncoding(CharsetAutoSwitch.dectedEncode(url));
//        NodeList list = parser.extractAllNodesThatMatch(filter);
//        for (int i = 0; i < list.size(); i++) {
//            LinkTag node = (LinkTag) list.elementAt(i);
//            //获取链接的目标网址
//            String link=node.extractLink();
//            if(link!=null && !link.trim().equals(""))
//            {
//                //将目标网址加入到该页面的所有网址列表中
//                links.add(link);
//            }
//        }
//
//        return links;
//    }
//
//    public static void main(String[] args) throws IOException, ParseException,
//            InterruptedException, ParserException {
//        String url = "http://struts.apache.org/";
//        //收入网站
//        addSite(url);
//        //搜有带有lucene词的网页
//        searcher("lucene");
//    }
//
//}
