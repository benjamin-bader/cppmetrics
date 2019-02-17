import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.LongAdder;
import java.util.ArrayList;
import java.util.List;

public class LongAdderBench {
    public static void main(String[] args) throws Exception {
        final int numIters = 1000000;
        final int numThreads = 24;

        final LongAdder adder = new LongAdder();

        long start = System.currentTimeMillis();
        List<Thread> threads = new ArrayList<>(numThreads);
        for (int i = 0; i < numThreads; ++i) {
            Thread t = new Thread() {
                @Override
                public void run() {
                    for (int i = 0; i < numIters; ++i) {
                        adder.add(1);
                    }
                }
            };
            t.start();
            threads.add(t);
        }

        for (Thread t : threads) {
            t.join();
        }

        long end = System.currentTimeMillis();
        System.out.println("Finished in " + (end - start) + " millis");
        System.out.println("Final count: " + adder.sum() + ", expected " + (numIters * numThreads));
    }
}
