import java.sql.*;
import java.util.List;

// If you are looking for Java data structures, these are highly useful.
// Remember that an important part of your mark is for doing as much in SQL (not Java) as you can.
// Solutions that use only or mostly Java will not receive a high mark.
import java.util.ArrayList;
//import java.util.Map;
//import java.util.HashMap;
//import java.util.Set;
//import java.util.HashSet;
public class Assignment2 extends JDBCSubmission {

    public Assignment2() throws ClassNotFoundException {

        Class.forName("org.postgresql.Driver");
    }

    @Override
    public boolean connectDB(String url, String username, String password) {
        // Implement this method!
        try{
            Connection conn = DriverManager.getConnection(url, username, password);
        } catch(SQLException se){
            System.err.println("SQL Exception." +
                    "<Message>: " + se.getMessage());
            return false;
        }
        return true;
    }

    @Override
    public boolean disconnectDB() {
        // Implement this method!
        if(conn != null) {
            try {
                conn.close();
            } catch (SQLException se) {
                System.err.println("SQL Exception." +
                    "<Message>: " + se.getMessage());
                return false;
            }
        }
        return true;
    }

    @Override
    public ElectionCabinetResult electionSequence(String countryName) {
        // Implement this method!
        List<Integer> elections = new ArrayList<Integer>();
        List<Integer> cabinets = new ArrayList<Integer>();

        try {
          String queryString = "select election.id as e_id, cabinet.id as c_id
                                from election
	                                   join country on country_id = country.id
	                                   join cabinet on election_id = election.id
                                where country.name = ?
                                order by extract(year from e_date) desc";
          PreparedStatement ps = conn.prepareStatement(queryString);
          ps.setString(1, countryName);
          ResultSet rs = ps.executeQuery();


          while (rs.next()){
            elections.add(rs.getInt("e_id"));
            cabinets.add(rs.getInt("c_id"));
          }

        } catch (SQLException se) {
            System.err.println("SQL Exception." +
                    "<Message>: " + se.getMessage());
        }
        return new ElectionCabinetResult(elections, cabinets);
    }

    @Override
    public List<Integer> findSimilarPoliticians(Integer politicianName, Float threshold) {
        // Implement this method!
        List<Integer> ids = new ArrayList<Integer>();

        try{
            String description = new String('');
            String queryString = "SELECT description FROM politician_president WHERE politician_president.id = ?";
            PreparedStatement ps = conn.prepareStatement(queryString);
            ps.setString(1, politicianName);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                description = rs.getString("description");
            }

            String queryString1 = "SELECT id, description FROM politician_president";
            PreparedStatement ps1 = conn.prepareStatement(queryString1);
            ResultSet rs1 = ps1.executeQuery();
            while (rs1.next()) {
                if(similarity(rs1.getString("description"), description) >= threshold) {
                    int id = rs1.getInt("id");
                    if(id != politicianName){
                        ids.add(id);
                    }
                }
            }
        }catch (SQLException se)
        {
            System.err.println("SQL Exception." +
                    "<Message>: " + se.getMessage());
        }

        return ids;
    }

    public static void main(String[] args) {
        // You can put testing code in here. It will not affect our autotester.
        System.out.println("Hello");
    }

}
