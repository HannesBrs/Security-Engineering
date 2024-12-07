import java.util.Random;

/**
 * This class simulates finding a Pre-Image and Collision
 * when generating random ints and counting the amount of
 * generated ints needed as well as the average.
 */
public class Simulator {

    private static int countPreimageTotal = 0;
    private static int countCollisionTotal = 0;
    private static double averageStepsPreimage = 0.0;
    private static double averageStepsCollision = 0.0;


    /**
     * This method finds Pre-Images in random generated ints
     * @param y the image that will be looked for
     * @param n the upper limit of int value
     */
    private static void findPreimage(int y, int n) {
        boolean preimageFound = false;

        int count = 0;
        Random rand = new Random();

        while (!preimageFound) {
            int randInt = rand.nextInt(n);
            count++;
            if (randInt == y) {
                System.out.println("Pre-Image found: x = " + randInt + "; y = " + y + " after " + count + " steps!");
                countPreimageTotal += count;
                preimageFound = true;
            }
        }
    }

    /**
     * This method finds Collisions in random generated ints
     * using a boolean array to look for duplicates
     * @param n the upper limit of int value
     */
    private static void findCollision(int n) {
        boolean collisionFound = false;
        boolean[] booleanArray = new boolean[n];
        for (int i = 0; i < n; i++) {
            booleanArray[i] = false;
        }

        int count = 0;
        Random rand = new Random();

        while (!collisionFound) {
            int randInt = rand.nextInt(n);
            count++;
            if (!booleanArray[randInt]) {
                booleanArray[randInt] = true;
            } else {
                System.out.println("Collision found: y = " + randInt + " after " + count + " steps!");
                countCollisionTotal += count;
                collisionFound = true;
            }
        }
    }

    /**
     * This method calculates the average steps / numbers generated for finding
     * a Pre-Image and a Collision
     * @param numSimulations the amount of simulations
     */
    private static void calculateAverageSteps(int numSimulations) {
        averageStepsPreimage = (double) countPreimageTotal / (double) numSimulations;
        averageStepsCollision = (double) countCollisionTotal / (double) numSimulations;
    }

    /**
     * This method executes finding the Pre-Image and Collisions multiple times
     * @param n the upper limit of int value
     * @param y the image that will be looked for
     * @param numSimulations the amount of simulations
     */
    public static void execute(int n, int y, int numSimulations) {
        for (int i = 0; i < numSimulations; i++) {
            findPreimage(y, n);
            findCollision(n);
        }

        calculateAverageSteps(numSimulations);

        System.out.println("\n-------------------------------------------------------------");
        System.out.println("Average steps required for Pre-Image: " + averageStepsPreimage);
        System.out.println("Average steps required for Collision: " + averageStepsCollision);
        System.out.println("-------------------------------------------------------------\n");
    }
}
