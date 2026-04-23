import java.io.*;
import java.net.*;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server {


    static List<PrintWriter> clients = new CopyOnWriteArrayList<>(); // потокобезопасный список клиентов
    static ExecutorService pool = Executors.newFixedThreadPool(5); // пул потоков

    public static void main(String[] args) throws Exception {

        ServerSocket server = new ServerSocket(8080);
        System.out.println("Server started...");

        // поток: ввод с сервера (консоль)
        new Thread(() -> {
            try {
                BufferedReader console = new BufferedReader(new InputStreamReader(System.in));
                while (true) {
                    String msg = console.readLine();
                    sendAll("SERVER: " + msg);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();

        // принимаем клиентов
        while (true) {
            Socket socket = server.accept();
            System.out.println("Client connected");

            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            clients.add(out);

            pool.execute(() -> {
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    String msg;
                    while ((msg = in.readLine()) != null) {
                        System.out.println("Client: " + msg);
                    }
                } catch (Exception e) {
                    System.out.println("Client disconnected");
                }
            });
        }
    }

    static void sendAll(String msg) {
        for (PrintWriter out : clients) {
            out.println(msg);
        }
    }
}