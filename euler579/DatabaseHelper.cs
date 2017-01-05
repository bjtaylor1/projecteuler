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

        public VectorInt[] GetAllPrimitivesOfSameSquare(VectorInt vector)
        {
            var vectors = new List<VectorInt>();
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
                        var v = new VectorInt(a,b,c);
                        if (!vector.Equals(v)) vectors.Add(v);
                    }
                }
            }
            return vectors.ToArray();
        }

        public void ResetDone()
        {
            using (var cmd = new SqlCommand("delete DoneTriples", sqlConnection))
            {
                cmd.ExecuteNonQuery();
            }
        }

        public bool IsDone(int[] vector)
        {
            using (var cmd = new SqlCommand(@"select count(*) from DoneTriples where A = @A and B = @B and C = @C", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@A", vector[0]);
                cmd.Parameters.AddWithValue("@B", vector[1]);
                cmd.Parameters.AddWithValue("@C", vector[2]);
                var count = Convert.ToInt32(cmd.ExecuteScalar());
                var isDone = count > 0;
                return isDone;
            }
        }

        public void SetDone(int[] vector)
        {
            using (var cmd = new SqlCommand(@"insert DoneTriples(A,B,C) values(@A,@B,@C)", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@A", vector[0]);
                cmd.Parameters.AddWithValue("@B", vector[1]);
                cmd.Parameters.AddWithValue("@C", vector[2]);
                cmd.ExecuteNonQuery();
            }
        }

        public static DatabaseHelper Instance { get; }= new DatabaseHelper();
        public void Dispose()
        {
            sqlConnection.Dispose();
        }
    }
}
