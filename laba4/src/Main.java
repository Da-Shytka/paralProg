import java.util.*;
import java.util.concurrent.*;

public class Main {

    public static final int THREADS = 50;
    public static final int ITERATIONS = 100000;
    public static final double NSEC = 1000_000_000.0;
    public static final int MAP_SIZE = 3;
    public static final int SAMPLES = 5;

    public static Map<String, Integer> hashMap = new HashMap<>();
    public static Map<String, Integer> hashTable = new Hashtable<>();
    public static Map<String, Integer> syncMap = Collections.synchronizedMap(new HashMap<>());
    public static Map<String, Integer> cHashMap = new ConcurrentHashMap<>();

    public static void main(String[] args) {

        System.out.println("Collections:");
        double hashMapTime = compute(hashMap) / NSEC;
        double hashTableTime = compute(hashTable) / NSEC;
        double syncMapTime = compute(syncMap) / NSEC;
        double cHashMapTime = compute(cHashMap) / NSEC;

        System.out.println("Execution times:");
        System.out.println(String.format("\tHashMap: %.3f s,\n\tHashTable: %.3f s,\n\tSyncMap: %.3f s,\n\tConcurrentHashMap: %.3f s.",
                hashMapTime, hashTableTime, syncMapTime, cHashMapTime));
    }

    private static long compute(Map<String, Integer> map) {

        System.out.print(String.format("\t%s", map.getClass().getName()));

        long start = 0;
        long stop = 0;

        for (int k = 0; k < SAMPLES; k++) {

            start = System.nanoTime();

            // подготовили инфраструктуру для записи
            ExecutorService executorService = Executors.newFixedThreadPool(THREADS);

            List<Callable<String>> tasks = new ArrayList<>();
            List<Future<String>> results = new ArrayList<>();

            // логика работы в каждом потоке
            for (int i = 0; i < THREADS; i++) {
                tasks.add(() -> {
                    String threadName = Thread.currentThread().getName();

                    Random random = new Random();

                    // большое количество операций для каждого потока
                    for (int s = 0; s < ITERATIONS; s++) {
                        String key = "key" + random.nextInt(MAP_SIZE);

                        // каждые 5 шагов поток засыпает (искусственная задержка)
//                        if (s % 5 == 0) {
//                            Thread.sleep(10);
//                        }

                        // берет значение по ключу, прибавляет 1 и записывает обратно
                        map.merge(key, 1, Integer::sum);

                    }
                    return "Thread " + threadName + " done";
                });
            }

            // зхапуск потоков одновременно
            try {
                results = executorService.invokeAll(tasks);
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }

            // ждем завершение работы таска
            try {
                for (Future<String> result : results) {
                    String s = result.get();
                    // System.out.println(s);
                }
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }

            executorService.shutdown();

            stop = System.nanoTime();
        }

        int actual = map.values().stream()
                .mapToInt(Integer::intValue)
                .sum();

        int expected = THREADS * ITERATIONS * SAMPLES;

        int lost = expected - actual;

        System.out.println("Expected: " + expected);
        System.out.println("Actual: " + actual);
        System.out.println("Lost: " + lost);


        System.out.println("...done.");

        return stop - start;
    }
}