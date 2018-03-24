import org.apache.commons.cli.*;

public class Main {
    public static void main(String args[]) {
        //MiniHttp miniHttp = new MiniHttp("::", 8888);
        Options options = new Options();

        Option addressOption = new Option("a", "address", true, "bind address");
        addressOption.setRequired(false);
        options.addOption(addressOption);

        Option portOption = new Option("p", "port", true, "bind port");
        portOption.setRequired(false);
        options.addOption(portOption);

        Option helpOption = new Option("h", "help", false, "print this message");
        helpOption.setRequired(false);
        options.addOption(helpOption);

        Option dirOption = new Option("d", "dir", true, "directory of the web server");
        dirOption.setRequired(false);
        options.addOption(dirOption);

        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmd;

        try {
            cmd = parser.parse(options, args);
        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("MiniHttp", options);
            System.exit(1);
            return;
        }

        if(cmd.hasOption("help")) {
            formatter.printHelp("MiniHttp", options);
            System.exit(0);
        }


        try {
            String address = cmd.getOptionValue("address", "0.0.0.0");
            int port = Integer.parseInt(cmd.getOptionValue("port", "80"));
            String dir = cmd.getOptionValue("dir", ".");
            new MiniHttp(address, port, dir);
        } catch(NumberFormatException e) {
            e.printStackTrace();
        }

    }
}
