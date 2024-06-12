import java.util.Scanner;
import java.text.DecimalFormat;

public class RetireRunner {
    public static void main(String[] args) throws Exception {

        RetirementEngine myPortfolio = new RetirementEngine();
        myPortfolio.loadHistReturns("HDI.txt");

        Scanner in = new Scanner(System.in);

        System.out.print("What percentage will be allocated to stocks?: ");
        int stockPer = in.nextInt();

        System.out.print("What percentage will be allocated to bonds?: ");
        int bondPer = in.nextInt();

        System.out.print("What percentage will be allocated to real estate?: ");
        int rePer = in.nextInt();

        System.out.println("The rest will be assumed to be parked in cash.");
        int cPer = (100 - stockPer - bondPer - rePer);

        System.out.print("How many years until retirement?: ");
        int yrsRetire = in.nextInt();

        int yrsLen = yrsRetire + 1;
        int initialYears = yrsLen - 1; // Save the initial years value

        int[] myDeposits = new int[yrsLen];

        System.out.print("How much money are you starting with? ");
        int initial = in.nextInt();

        in.nextLine(); // To consume the rest of the line

        System.out.print("How old are you?: ");
        int age = in.nextInt();

        in.nextLine(); // To consume the rest of the line

        System.out.print("What is your gender?: Options (Male) (Female): ");
        String gender = in.nextLine();

        System.out.print("What is your race?: Options (White) (Hispanic) (Native American) (Asian) (Black): ");
        String race = in.nextLine();

        int mwhite = 74, mhispanic = 74, mblack = 67, mnative = 62, masian = 81;
        int fwhite = 79, fhispanic = 81, fblack = 75, fnative = 69, fasian = 86;
        int newRace = 0;

        if (gender.equalsIgnoreCase("Male")) {
            switch (race) {
                case "White":
                    newRace = mwhite;
                    break;
                case "Hispanic":
                    newRace = mhispanic;
                    break;
                case "Native American":
                    newRace = mnative;
                    break;
                case "Asian":
                    newRace = masian;
                    break;
                case "Black":
                    newRace = mblack;
                    break;
                default:
                    System.out.println("Invalid race entered.");
                    break;
            }
        } else if (gender.equalsIgnoreCase("Female")) {
            switch (race) {
                case "White":
                    newRace = fwhite;
                    break;
                case "Hispanic":
                    newRace = fhispanic;
                    break;
                case "Native American":
                    newRace = fnative;
                    break;
                case "Asian":
                    newRace = fasian;
                    break;
                case "Black":
                    newRace = fblack;
                    break;
                default:
                    System.out.println("Invalid race entered.");
                    break;
            }
        } else {
            System.out.println("Invalid gender entered.");
        }

        myDeposits[0] = initial;

        System.out.println("Your life expectancy based on gender and race is: " + newRace + " years.");

        int i = 1;
        while (i < yrsLen) {

            System.out.println("How much are you going to put away each year (up to " + initialYears + " years)?: ");
            int mtemp = in.nextInt();

            System.out.println("For how many years?: ");
            int ytemp = in.nextInt();

            if (i + ytemp > yrsLen) {
                System.out.println("You entered too many years. Please enter up to " + (yrsLen - i) + " years");
                ytemp = in.nextInt();
            }

            for (int j = 0; j < ytemp && i < yrsLen; j++, i++) {
                myDeposits[i] = mtemp;
            }
        }

        System.out.println("How many times would you like to run this analysis?");
        int lifeTimes = in.nextInt();

        myPortfolio.canIRetire(stockPer, bondPer, myDeposits, lifeTimes, rePer);
    }
}
