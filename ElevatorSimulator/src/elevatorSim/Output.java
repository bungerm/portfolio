package elevatorSim;

import java.util.concurrent.TimeUnit;

public class Output {
    private static long startTime = System.nanoTime();
    private static long initTime = System.currentTimeMillis();

    public static String timeStamp() {
        /*
        long difference = System.nanoTime() - startTime;
        String myString = String.format("%02d:%02d:%02d", TimeUnit.NANOSECONDS.toMinutes(difference)
                % TimeUnit.HOURS.toMinutes(1), TimeUnit.NANOSECONDS.toSeconds(difference)
                % TimeUnit.MINUTES.toSeconds(1), TimeUnit.NANOSECONDS.toMillis(difference)
                % TimeUnit.SECONDS.toMillis(1));
        */
        long currentTime = System.currentTimeMillis() - initTime;
        long hours = currentTime / 3600000;
        currentTime -= hours * 3600000;
        long minutes = currentTime / 60000;
        currentTime -= minutes * 60000;
        long seconds = currentTime / 1000;
        String newString = String.format("%02d:%02d:%02d", hours, minutes, seconds);

        return newString;
    }
}
