import java.math.BigDecimal;
import java.util.function.BiFunction;

interface NumericOperator extends BiFunction<BigDecimal, BigDecimal, BigDecimal> {
}
