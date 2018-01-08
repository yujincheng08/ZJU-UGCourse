package Client;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MD5Encoder {
    private static final String DEFAULT_SALT = "MiNiQQ";
    static String encode(String str, String salt) {
        try {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            byte[] digest = md5.digest((str + salt + str.length()).getBytes());
            BigInteger bigInteger = new BigInteger(1, digest);
            StringBuilder hash = new StringBuilder(bigInteger.toString(16));
            while (hash.length() < 32)
                hash.insert(0, "0");
            return hash.toString();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return null;
    }

    static String encode(String str) {
        return encode(str, DEFAULT_SALT);
    }
}
