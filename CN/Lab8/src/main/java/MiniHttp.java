import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.ServerSocket;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

class MiniHttp {
    private static final int BACKLOG = 50;
    private File home;
    private ThreadPoolExecutor handlerPool = (ThreadPoolExecutor) Executors.newFixedThreadPool(BACKLOG);

    private boolean checkDir(String path) {
        home = new File(path);
        return home.exists() && home.canRead();
    }

    MiniHttp(String address, int port, String dir) {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            handlerPool.shutdown();
            System.out.println("Good Bye!");
        }));
        if(!checkDir(dir))
            System.err.println("Permission denied to access directory " + dir);
        try (ServerSocket serverSocket = new ServerSocket(port, BACKLOG, InetAddress.getByName(address))) {
            System.out.printf("Server listen on: %s:%d, with root directory: %s\n", address, port, dir);
            //noinspection InfiniteLoopStatement
            while(true) {
                Socket client = serverSocket.accept();
                handlerPool.execute(new RequestHandler(client, home));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
