import java.io.*;
import java.net.*;

public class Client {

    public static void main(String[] args) throws Exception {

        Socket socket = new Socket("localhost", 8080);
        System.out.println("Connected to server");

        BufferedReader console = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

        // поток: слушаем сервер
        new Thread(() -> {
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String msg;
                while ((msg = in.readLine()) != null) {
                    System.out.println(msg);
                }
            } catch (Exception e) {
                System.out.println("Server disconnected");
            }
        }).start();

        // отправка сообщений серверу
        while (true) {
            String msg = console.readLine();
            out.println(msg);
        }
    }
}