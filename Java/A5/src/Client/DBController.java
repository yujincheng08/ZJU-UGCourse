package Client;

import java.sql.*;
import java.time.LocalDateTime;

public class DBController {

    private Connection connection;

    DBController(String path)
            throws SQLException
    {
        connection = DriverManager.getConnection("jdbc:sqlite:" + path);
    }

    public boolean checkTable(String tableName)
            throws SQLException
    {
        PreparedStatement statement =  connection.prepareStatement("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name=?");
        statement.setString(1, tableName);
        ResultSet result = statement.executeQuery();
        return result.getInt(1) > 0;
    }

    public boolean createChartLogTable(String userID)
            throws SQLException
    {
        Statement statement = connection.createStatement();
        statement.execute(
                "CREATE TABLE \""+userID+ "\" (content TEXT NOT NULL, timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL );");
        return checkTable(userID);
    }

    public boolean checkAndCreateChartLogTable(String userID)
            throws SQLException {
        return checkTable(userID) || createChartLogTable(userID);
    }

    public void newChartLog(String userID, String content)
            throws SQLException
    {
        checkAndCreateChartLogTable(userID);
        PreparedStatement statement = connection.prepareStatement("INSERT INTO \"" + userID + "\" VALUES (?, NULL);");
        statement.setString(1, content);
        statement.execute();
    }

    public String [] getChartLog(String userID, Timestamp endTime, int limit)
            throws SQLException
    {
        if(!checkAndCreateChartLogTable(userID))
            return null;
        PreparedStatement statement = connection.prepareStatement("SELECT timestamp, content FROM \"" + userID + "\" WHERE timestamp < ? ORDER BY timestamp ASC;");
        statement.setTimestamp(1, endTime);
        //statement.setInt(2, limit);
        ResultSet result = statement.executeQuery();
        while(result.next())
        {
            System.out.println("timestamp: " + result.getTimestamp(1).toString() + " content:"+result.getString(2));
        }
        return null;
    }

    public static void main(String []arg)
    {
        try {
            DBController dbController = new DBController("./db.db");
            dbController.newChartLog("1","Hello!");
            dbController.getChartLog("1", Timestamp.valueOf(LocalDateTime.now()), 10);
        }catch(SQLException e)
        {
            e.printStackTrace();
        }
    }

}
