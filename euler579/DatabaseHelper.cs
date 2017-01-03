using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    public class DatabaseHelper : IDisposable
    {
        private SqlConnection sqlConnection;

        public DatabaseHelper()
        {
            sqlConnection = new SqlConnection("Initial Catalog=triples;Integrated Security=SSPI");
            sqlConnection.Open();
        }

        public Vector3D[] GetAllPrimitivesOfSameSquare(Vector3D vector)
        {
            var vectors = new List<Vector3D>();
            using (var cmd = new SqlCommand("select A,B,C from primitivetriples where [square] = @square", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@square", vector.Length);
                using (var rs = cmd.ExecuteReader())
                {
                    while (rs.Read())
                    {
                        var a = (int) rs["A"];
                        var b = (int) rs["B"];
                        var c = (int) rs["C"];
                        var v = new Vector3D(a,b,c);
                        if (!vector.Equals(v)) vectors.Add(v);
                    }
                }
            }
            return vectors.ToArray();
        }

        public static DatabaseHelper Instance { get; }= new DatabaseHelper();
        public void Dispose()
        {
            sqlConnection.Dispose();
        }
    }
}
