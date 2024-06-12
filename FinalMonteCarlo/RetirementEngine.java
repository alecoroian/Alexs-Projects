import java.util.Scanner;
import java.io.File;
import java.text.DecimalFormat;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Random;
import java.util.Collections;

/**
 * Write a description of class RetireRunner here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class RetirementEngine {
    private ArrayList<YearEconData> returnDataGold;

    public RetirementEngine() {
        returnDataGold = new ArrayList<YearEconData>();
    }

    public void loadHistReturns(String inputFile) throws Exception {
        File myFile = new File(inputFile);
        Scanner myScan = new Scanner(myFile);

        while (myScan.hasNextLine()) {
            String myLine = myScan.nextLine();
            String[] myFields = myLine.split(",");
            // System.out.println(Arrays.toString(myFields));

            YearEconData myYear = new YearEconData(myFields);
            // System.out.println(myYear);
            returnDataGold.add(myYear);
        }
    }

    public double oneLyfeTime(int spAlloc, int bondAlloc, int[] deposits, int reAlloc) {
        ArrayList<YearEconData> returnData = new ArrayList<YearEconData>(returnDataGold);
        int mmAlloc = 100 - spAlloc - bondAlloc - reAlloc;
        double total = 0;
        Random myRand = new Random();
        for (int i = 0; i < deposits.length; i++) {
            int randyr = myRand.nextInt(returnData.size());
            double yrSP = returnData.get(randyr).getsp();
            double yrBOND = returnData.get(randyr).getbond();
            double yrMM = returnData.get(randyr).getmm();
            double yrINF = returnData.get(randyr).getinf();
            double yrRE = returnData.get(randyr).getre();

            total += deposits[i];
            double tempSP = (total * (spAlloc / 100.0) * (yrSP + 1));
            double tempMM = (total * (mmAlloc / 100.0) * (yrMM + 1));
            double tempBond = (total * (bondAlloc / 100.0) * (yrBOND + 1));
            double tempRE = (total * (reAlloc / 100.0) * (yrRE + 1));
            total = tempSP + tempMM + tempBond + tempRE;
            total -= total * yrINF;
            returnData.remove(randyr);
        }
        return total;
    }

    public void canIRetire(int spAlloc, int bondAlloc, int[] deposits, int lifeTimes, int reAlloc) {
        ArrayList<Double> oneLyfe = new ArrayList<Double>();
        for (int j = 0; j < lifeTimes; j++) {
            double oneLyfeReturn = oneLyfeTime(spAlloc, bondAlloc, deposits, reAlloc);
            oneLyfe.add(oneLyfeReturn);
        }
        Collections.sort(oneLyfe);
        System.out.println();

        int decile = (int) (oneLyfe.size() * .10);
        double total = 0;
        for (int i = decile; i < oneLyfe.size() - decile; i++) {
            total = total + oneLyfe.get(i);
        }
        double average = total / (oneLyfe.size() - 2 * decile);

        DecimalFormat df = new DecimalFormat("#,###.00");
        System.out.println("The average retirement savings would be $" + df.format(average));
        System.out.println();
        System.out.println("Assuming all additions are made at the end of any given year.");
    }
}