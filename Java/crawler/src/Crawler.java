import java.io.*;
import java.net.URL;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Crawler {
    public static void main(String[] args) {
        String baseUrl = "http://www.00ksw.com/html/1/1343/";
        String nextUrl = "597361.html";
        String destFilePath = "E:\\test.txt";
        System.out.println("开始爬取数据...");
        long startTime = System.currentTimeMillis();
        getNovel(baseUrl, nextUrl, destFilePath);
        long endTime = System.currentTimeMillis();
        System.out.println("用时 " + (endTime - startTime) / 1000 + "秒...");
    }

    private static void getNovel(String baseUrl, String nextUrl, String destFilePath) {
        try{
            File destFile = new File(destFilePath);

            if (!destFile.exists()) {
                destFile.delete();
            }
            destFile.createNewFile();

            FileWriter fw = new FileWriter(destFile);
            String realUrl, resultContent;
            StringBuilder sb = new StringBuilder();
            BufferedReader in = null;
            Pattern contentPat = Pattern.compile("<div id=\"content\">(.+?)</div>");
            Pattern titlePat = Pattern.compile("readtitle = \"(.+?)\"");
            Pattern nextPat = Pattern.compile("&rarr; <a href=\"(.+?)\">");
            Matcher matcher;

            while (!nextUrl.startsWith("/")) {
                realUrl = baseUrl + nextUrl;
                in = new BufferedReader(new InputStreamReader(new URL(realUrl).openStream()));
                String line;
                while ((line = in.readLine()) != null) {
                    System.out.println(line);
                    sb.append(line);
                }
                resultContent = sb.toString().replaceAll("&nbsp;", "").replaceAll("<br /><br />", "*****");
                fw.write("\r\n\r\n");
                matcher = titlePat.matcher(resultContent);
                if (matcher.find()) {
                    fw.write(matcher.group(1));
                }
                fw.write("\r\n");

                matcher = contentPat.matcher(resultContent);
                if (matcher.find()) {
                    fw.write(matcher.group(1));
                }

                matcher = nextPat.matcher(resultContent);
                if (matcher.find()) {
                    nextUrl = matcher.group(1);
                }

                sb.delete(0, sb.length());
            }
            if (in != null) {
                in.close();
            }
            fw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

