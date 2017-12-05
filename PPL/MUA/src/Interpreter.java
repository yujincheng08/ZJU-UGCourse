public class Interpreter
{
    public static void main(String [] args)
    {
        WordList globalWordList = new WordList();
        WordStream wordStream = new WordStream();
        interpret(wordStream, globalWordList);
        if(globalWordList.contains(Function.outputWordName))
        {
            try
            {
                System.out.println("Program exited with " + globalWordList.thing(Function.outputWordName).toString());
            }
            catch (Throwable e)
            {
                e.printStackTrace();
            }
        }
    }


    private static Boolean isWordLiterary(String name)
    {
        return name.startsWith("\"");
    }

    private static Value getValue(String operator, WordStream stream, WordList localWordList)
            throws RunningException, SyntaxException
    {
        switch (operator)
        {
            case "thing":
                return Function.thing(stream, localWordList);
            case "isname":
                return Function.isName(stream, localWordList);
            case "read":
                return Function.read(stream);
            case "readlinst":
                return Function.readLinst();
            case "add":
                return Function.add(stream, localWordList);
            case "sub":
                return Function.sub(stream, localWordList);
            case "mul":
                return Function.mul(stream, localWordList);
            case "div":
                return Function.div(stream, localWordList);
            case "mod":
                return Function.mod(stream, localWordList);
            case "eq":
                return Function.eq(stream, localWordList);
            case "gt":
                return Function.gt(stream, localWordList);
            case "lt":
                return Function.lt(stream, localWordList);
            case "and":
                return Function.and(stream, localWordList);
            case "or":
                return Function.or(stream, localWordList);
            case "not":
                return Function.not(stream, localWordList);
            /*
            case "pi":
                return new Value("3.1415926535897932");
             */
            default:
                if(operator.startsWith(":"))
                    return Function.thing(operator.substring(1), localWordList);
                else if(isWordLiterary(operator))
                    return new Value(operator.substring(1));
                else if(operator.matches("[+-]?\\d+(\\.\\d+)?"))
                    return new Value(operator);
                else if(operator.equals("["))
                    return new Value(operator, stream, localWordList);
                else if(operator.equals("("))
                    return new Value(operator, stream, localWordList);
                else
                    //throw new SyntaxException("Unexpect token: " + next);
                    //throw new RunningException("Undefined function: " + next);
                    return Function.run(operator, stream, localWordList);
        }
    }

    static Value value(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        String operator = stream.next();
        if(operator == null)
            throw new RunningException("Expected a value but got null");
        Value val = getValue(operator, stream, wordList);
        if(val == null)
            throw new RunningException(operator + " does not return a value");
        return val;
    }

    private static void printResult(WordStream stream, WordList wordList)
            throws RunningException, SyntaxException
    {
        String operator = stream.next();
        if(operator == null)
            throw new RunningException("Expected a value but got null");
        if (operator.equals("[")) interpret((new Value(operator, stream, wordList).toWordStream()), wordList);
        else if (operator.startsWith(":")) interpret(Function.thing(operator.substring(1), wordList).toWordStream(), wordList);
        else {
            Value val = getValue(operator, stream, wordList); //Function calls
            if(val != null) {
                stream = val.toWordStream();
                operator = stream.next();
                if(val.size()>1)
                    interpret(stream, wordList);
                else if (operator.startsWith(":")) interpret(Function.thing(operator.substring(1), wordList).toWordStream(), wordList);
                else System.out.println(val.toString());

            }
        }
    }

    static void interpret(WordStream stream, WordList localWordList)
    {
        mainLoop: while(true)
        {
            String command = stream.next();
            try
            {
                if(command == null) {
                    break;
                }
                switch (command)
                {
                    case "make":
                        Function.make(stream, localWordList);
                        break;
                    case "print":
                        Function.print(stream, localWordList);
                        break;
                    case "erase":
                        Function.erase(stream, localWordList);
                        break;
                    case "thing":
                        interpret(Function.thing(stream, localWordList).toWordStream(), localWordList);
                        break;
                    case "output":
                        Function.output(stream, localWordList);
                        break;
                    case "repeat":
                        Function.repeat(stream, localWordList);
                        break;
//            case "if":
//                break;
//            case "run":
//                break;
                    case "stop":
                        break mainLoop;
                    default:
                        //else Function.run(command, stream);
                        //interpret(value(stream).toWordStream());
                        printResult(stream.putBack(command), localWordList);
                        //else throw new SyntaxException("Unexpected token: " + command);
                }
            } catch (RunningException e)
            {
                System.out.println("Runtime Error: " + e.getMessage());
            } catch (SyntaxException e)
            {
                System.out.println("Syntax Error: " + e.getMessage());
            }
        }
    }
}
