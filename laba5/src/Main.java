import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Main {

    public static final int THREADS = 4;
    public static final int COUNT = 2;
    public static Semaphore regularSemaphore = new Semaphore(COUNT);
    public static MySemaphore mySemaphore = new MySemaphore(COUNT);
    public static SpinSemaphore spinSemaphore = new SpinSemaphore(COUNT);

    public static void main(String[] args) {

        System.out.println("=== Обычный Semaphore ===");
        runTask(regularSemaphore);

        System.out.println("\n=== Мой Semaphore (Lock + Condition) ===");
        runTask(mySemaphore);

        System.out.println("\n=== Spin Semaphore (CAS) ===");
        runSpinTask();
    }

    private static void runTask(Semaphore semaphore) {
        ExecutorService es = Executors.newFixedThreadPool(THREADS);
        List<Callable<String>> tasks = new ArrayList<>();

        for (int i = 0; i < THREADS; i++) {
            tasks.add(() -> {
                String name = Thread.currentThread().getName();
                System.out.println(name + " пытается войти...");
                try {
                    semaphore.acquire();
                    System.out.println(name + " вошёл");

                    Thread.sleep(2000);

                    System.out.println(name + " выходит");
                    semaphore.release();

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                return "done";
            });
        }
        try {
            es.invokeAll(tasks);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        es.shutdown();
    }

    private static void runSpinTask() {
        ExecutorService es = Executors.newFixedThreadPool(THREADS);
        List<Callable<String>> tasks = new ArrayList<>();

        for (int i = 0; i < THREADS; i++) {
            tasks.add(() -> {
                String name = Thread.currentThread().getName();
                System.out.println(name + " пытается войти...");

                spinSemaphore.acquire();
                System.out.println(name + " вошёл");

                Thread.sleep(2000);

                System.out.println(name + " выходит");
                spinSemaphore.release();

                return "done";
            });
        }
        try {
            es.invokeAll(tasks);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        es.shutdown();
    }
}