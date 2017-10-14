public class Interpreter
{
    private static WordStream wordStream;
    public static void main(String [] args)
    {
        wordStream = new WordStream();
        while(true)
            interpret(wordStream);
    }


    public static Boolean isWordLiterary(String name)
    {
        return name.startsWith("\"");
    }

    public static Value value(WordStream stream)
            throws RunningException, SyntaxException
    {
        String next = stream.next();
        switch (next)
        {
            case "thing":
                return Function.thing(stream);
            case "isname":
                return Function.isName(stream);
            case "read":
                return Function.read();
            case "readlinst":
                return Function.readLinst();
            case "add":
                return Function.add(stream);
            case "sub":
                return Function.sub(stream);
            case "mul":
                return Function.mul(stream);
            case "div":
                return Function.div(stream);
            case "mod":
                return Function.mod(stream);
            case "eq":
                return Function.eq(stream);
            case "gt":
                return Function.gt(stream);
            case "lt":
                return Function.lt(stream);
            case "and":
                return Function.and(stream);
            case "or":
                return Function.or(stream);
            case "not":
                return Function.not(stream);
            /*
            case "pi":
                return new Value("3.1415926535897932");
             */
            default:
                if(next.startsWith(":"))
                    return Function.thing(next.substring(1));
                else if(isWordLiterary(next))
                    return new Value(next.substring(1));
                else if(next.matches("-?\\d+(\\.\\d+)?"))
                    return new Value(next);
                else if(next.startsWith("["))
                    return new Value(next, stream);
                else
                    throw new SyntaxException("Unexpect token: " + next);
                    //throw new RunningException("Undefined function: " + next);
        }
    }

    public static void interpret(WordStream stream)
    {
        String command = stream.next();
        try
        {
            switch (command)
            {
                case "make":
                    Function.make(stream);
                    break;
                case "print":
                    Function.print(stream);
                    break;
                case "erase":
                    Function.erase(stream);
                    break;
                case "thing":
                    interpret(Function.thing(stream).toWordStream());
                    break;
//            case "if":
//                break;
//            case "run":
//                break;
                default:
                    if(command.startsWith("["))
                        interpret((new Value(command, stream).toWordStream()));
                    else if(command.startsWith(":"))
                        interpret(Function.thing(command.substring(1)).toWordStream());//interpret(value(stream).toWordStream());
                    else
                        throw new SyntaxException("Unexpected token: " + command);
            }
        } catch(RunningException e)
        {
            System.out.println("Runtime Error: " + e.getMessage());
        } catch(SyntaxException e)
        {
            System.out.println("Syntax Error: " + e.getMessage());
        }
    }
}
