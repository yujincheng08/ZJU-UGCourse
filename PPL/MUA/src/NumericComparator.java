import java.math.BigDecimal;
import java.util.function.BiFunction;

interface NumericComparator extends BiFunction<BigDecimal, BigDecimal, Boolean> {
}
