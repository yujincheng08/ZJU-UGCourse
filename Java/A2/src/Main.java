import java.util.Scanner;
class Main {
    public static void main(String [] args)
    {
        Scanner in = new Scanner(System.in);
        int hintNum = in.nextInt();
        Sodoku sodoku = new Sodoku();
        sodoku.generate(hintNum);
        sodoku.print();
        System.out.println("Answer is:");
        sodoku.print(true);
    }
}
