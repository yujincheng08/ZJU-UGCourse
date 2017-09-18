import java.util.InputMismatchException;
import java.util.Scanner;
public class Main {
    private static Scanner in;
    public static void main(String args[]) {
        in = new Scanner(System.in);
        mainLoop:
        while (true) {
            System.out.println("Please select your operation:");
            System.out.println("1. Hex number to Dec number");
            System.out.println("2. Dec number to Hex number");
            System.out.println("3. Exit");
            int op = in.nextInt();
            switch (op) {
                case 3:
                    break mainLoop;
                case 1:
                    Hex2Dec();
                    break;
                case 2:
                    Dec2Hex();
                    break;
                default:
                    System.out.println("Invalid input");
            }
        }
        in.close();
    }

    private static void Dec2Hex() throws InputMismatchException
    {
        System.out.print("Please enter the number you want to convert: ");
        int num = in.nextInt();
        System.out.print("The result is: ");
        String result = "";
        if(num < 0)
        {
            num = -num;
            System.out.print('-');
        }
        do
        {
            result = toHex(num%16) + result;
            num/=16;
        }while(num != 0);
        System.out.println(result);
    }

    private static char toHex(int n)
    {
        if(n>=0 && n <=9)
            return (char)(n + '0');
        else if(n>=10 && n <=15)
            return (char)('A' + n - 10);
        else return (char)(0);
    }

    private static void Hex2Dec()
    {
        System.out.print("Please enter the number you want to convert: ");
        String hex = in.next("-?[0-9A-Fa-f]+");
        System.out.print("The result is: ");
        int i = 0;
        if(hex.charAt(i)=='-')
        {
            System.out.print("-");
            ++i;
        }
        int result = 0;
        for(;i<hex.length();++i)
        {
            result = result * 16 + fromHex(hex.charAt(i));
        }
        System.out.println(result);
    }

    private static int fromHex(char c)
    {
        if(c>='0' && c <='9')
            return c - '0';
        else if(c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        else if(c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        return 0;
    }
}
