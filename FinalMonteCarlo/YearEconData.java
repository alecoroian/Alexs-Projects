public class YearEconData {
    /**
     * instance variables
     */
    private int yr;
    private double sp;// s&p return
    private double mm;// money market return
    private double bond;// bond return
    private double inf;// inflation
    private double re;// real estate

    /**
     * constructors
     */
    public YearEconData(String[] fields) {
        yr = Integer.parseInt(fields[0]);
        sp = Double.parseDouble(fields[1]);
        mm = Double.parseDouble(fields[2]);
        bond = Double.parseDouble(fields[3]);
        inf = Double.parseDouble(fields[4]);
        re = Double.parseDouble(fields[5]);
    }

    /**
     * methods
     */
    public int getYear() {
        return yr;
    }

    public void setYear(int year) {
        yr = year;
    }

    public double getsp() {
        return sp;
    }

    public void setsp(double spReturn) {
        sp = spReturn;
    }

    public double getmm() {
        return mm;
    }

    public void setmm(double mmReturn) {
        mm = mmReturn;
    }

    public double getbond() {
        return bond;
    }

    public void setbond(double bondReturn) {
        bond = bondReturn;
    }

    public double getinf() {
        return inf;
    }

    public void setinf(double infReturn) {
        inf = infReturn;
    }

    public double getre() {
        return re;
    }

    public void setre(double reReturn) {
        re = reReturn;
    }

    public String toString() {
        return "YearEconData{" + "year = " + yr + ", S&P return = " + sp + ", money market return = " + mm
                + ", bond return = " + bond + "inflation return = " + inf + "}";
    }
}
