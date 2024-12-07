import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * This class is an interactive dialogue class for the
 * Pre-Image and Collision simulator
 */
public class Dialogue {

    private Scanner input;
    private final int SIMULATE = 1;
    private final int END = 0;
    private static final String MENU =
                    "\n" +
                    "1 -> start simulation\n" +
                    "0 -> exit\n";

    private static final String EXIT_PROGRAM = "exit program...";
    private static final String INVALID_MENU_INPUT = "invalid menu option input";

    /**
     * Constructs a new dialogue, initializing the input Scanner for user interaction.
     * @see Scanner
     */
    public Dialogue() {
        input = new Scanner(System.in);
    }

    /**
     * Starts the dialogue, allowing users to choose to simulate or end the program.
     */
    private void startProgram() {
        int function = 0;

        do {
            try {
                printMenu();
                function = functionInput();
                startFunction(function);
            } catch (IllegalArgumentException e) {
                System.out.println(e);
            } catch (InputMismatchException e) {
                System.out.println(e);
                input.nextLine();
            } catch (Exception e) {
                System.out.println(e);
                e.printStackTrace(System.out);
            }
        } while (function != END);
    }

    /**
     * Reads the user's input for the chosen function.
     * @return The user's chosen function.
     */
    private int functionInput() {
        int function = input.nextInt();
        input.nextLine();
        return function;
    }

    /**
     * Prints the menu options for the dialogue program.
     */
    private void printMenu() {
        System.out.println(MENU);
    }

    /**
     * Executes the chosen function based on the user's input.
     * @param function The user's chosen function.
     */
    private void startFunction(int function) {
        switch (function) {
            case SIMULATE:
                receiveParameterInputsAndExecute();
            case END:
                System.out.println(EXIT_PROGRAM);
                break;
            default:
                System.out.println(INVALID_MENU_INPUT);
                break;
        }
    }

    /**
     * This method receives user inputs for setting
     * -the upper limit n
     * -the value for which is looked for during the Pre-Image simulation
     * -the amount of times the simulation is repeated
     * It then starts the simulator
     */
    private void receiveParameterInputsAndExecute() {
        System.out.println("Please enter the upper limit n!");
        int n = input.nextInt();
        if (n < 1) {
            throw new IllegalArgumentException("The upper limit n must be a positive number!");
        }

        System.out.println("Please enter a value for the Pre-Image simulation!");
        int y = input.nextInt();
        if (y < 1 || y >= n) {
            throw new IllegalArgumentException("The Pre-Image value must be less than the upper limit and a positive number!");
        }

        System.out.println("Please enter the number of simulations!");
        int numSimulations = input.nextInt();
        if (numSimulations < 1) {
            throw new IllegalArgumentException("The number of simulations must be a positive number!");
        }

        Simulator.execute(n, y, numSimulations);
    }

    /**
     * Creates the Simulator dialogue and initiates the method startProgram().
     * @param args The command-line arguments.
     */
    public static void main(String[] args) {
        new Dialogue().startProgram();
    }
}
