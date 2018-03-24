import com.sun.xml.internal.ws.policy.privateutil.PolicyUtils;

import java.io.*;
import java.net.Socket;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RequestHandler implements Runnable {
    private File home;
    private Socket client;
    private static final HashMap<String, String> supportedExtension;

    private static final String EXPECTED_LOGIN="3150101155";
    private static final String EXPECTED_PASS="1155";

    private String readLine(InputStream inputStream) throws IOException {
        StringBuilder line = new StringBuilder();
        int t;
        char c;
        while((t = inputStream.read()) != -1){
            c = (char) t;
            if(c != '\r')
                line.append(c);
            else {
                t = inputStream.read();
                if(t != '\n')
                    throw new IOException("Error Format");
                break;
            }
        }
        return line.toString();
    }

    static {
        HashMap<String, String> tmpMap = new HashMap<>();
        tmpMap.put("gif", "image/gif");
        tmpMap.put("png", "image/png");
        tmpMap.put("jpg", "image/jpg");
        tmpMap.put("jpeg", "image/jpeg");
        tmpMap.put("ico", "image/ico");
        tmpMap.put("zip", "image/zip");
        tmpMap.put("gz", "image/gz");
        tmpMap.put("tar", "image/tar");
        tmpMap.put("htm", "text/htm");
        tmpMap.put("html", "text/html");
        tmpMap.put("js", "application/javascript");
        tmpMap.put("txt", "text/plain");
        supportedExtension = tmpMap;
    }

    RequestHandler(Socket socket, File dir) {
        home = dir;
        client = socket;
    }

    private void printRequest(int code, String status, String meta, String data) throws IOException {
        printRequest(code, status, meta, data.getBytes("GBK"));
    }

    private void printRequest(int code, String status, String meta, byte[] data) throws IOException {
        OutputStream outputStream = client.getOutputStream();
        PrintWriter writer = new PrintWriter(outputStream);
        writer.print("HTTP/1.1");
        writer.print(code);
        writer.println(" " + status);
        writer.println("Server: 2nweb: a tiny, safe Web server (static pages only)");
        writer.println("Content-Length: " + data.length);
        writer.println("Connection: close");
        writer.print("Content-Type: ");
        writer.println(meta);
        writer.println();
        writer.flush();
        outputStream.write(data);
        outputStream.flush();
        client.close();
    }

    private void handleGet(String path) {
        if (path.equals("/"))
            path = "index.html";
        String suffix = path.substring(path.lastIndexOf(".") + 1);
        if (!supportedExtension.containsKey(suffix)) {
            error(415, "Unsupported file");
            return;
        }
        String meta = supportedExtension.get(suffix);
        File file = new File(home, path);
        if (!file.exists() || path.contains("..")) {
            error(404, "Not Found");
            return;
        }
        try {
            printRequest(200, "OK", meta, Files.readAllBytes(file.toPath()));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private boolean login(String data) {
        Pattern pattern = Pattern.compile("login=(?<login>.*)&pass=(?<pass>.*)");
        Matcher matcher = pattern.matcher(data);
        return matcher.matches() && matcher.group("login").equals(EXPECTED_LOGIN) && matcher.group("pass").equals(EXPECTED_PASS);
    }


    private void handlePost(String path, ArrayList<String> headers){
        try {
            int contentLength = -1;
            for (String header : headers) {
                if (header.toLowerCase().startsWith("content-length: ")) {
                    try {
                        contentLength = Integer.parseInt(header.substring(16));
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                        return;
                    }
                }
            }
            if(contentLength<0) return; //TODO: Error prompt
            byte[] chars = new byte[contentLength];
            if (contentLength < client.getInputStream().read(chars, 0, contentLength)) {
                System.err.println("error");
            }
            if (!path.equals("/dopost"))
                error(404, "Not Found");
            String data = new String(chars);
            String resonse = login(data)?"登录成功":"登录失败";
            printRequest(200, "OK", "text/html", "<html><body>"+resonse+"</body></html>" );

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void error(int code, String message) {
        try {
            printRequest(code, "Not Found", "text/plain", message);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void handleRequest(ArrayList<String> headers) {
        if (headers.isEmpty()) return;
        Pattern headerPattern = Pattern.compile("(?<method>(\\S+))\\s+(?<path>\\S+)\\s+.*");
        Matcher headerMatcher = headerPattern.matcher(headers.get(0));
        if (!headerMatcher.matches()) return;
        String method = headerMatcher.group("method");
        String path = headerMatcher.group("path");
        switch (method) {
            case "GET":
                handleGet(path);
                break;
            case "POST":
                handlePost(path, headers);
                break;
            default:
                error(405, "Unsupported method");
                break;
        }
    }

    @Override
    public void run() {
        Runtime.getRuntime().addShutdownHook(new Thread(()-> {
            if(client != null) {
                try {
                    client.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }));
        try {
            InputStream inputStream = client.getInputStream();
            String tmp;
            ArrayList<String> headers = new ArrayList<>();
            while (!(tmp = readLine(inputStream)).equals("")) {
                headers.add(tmp);
            }
            handleRequest(headers);
            client.close();
            client = null;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
