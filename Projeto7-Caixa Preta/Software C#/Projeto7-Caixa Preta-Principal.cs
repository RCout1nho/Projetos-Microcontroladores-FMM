using System;
using System.Windows.Forms;
using FireSharp.Config;
using FireSharp.Interfaces;
using FireSharp.Response;

namespace Parcial2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        IFirebaseConfig config = new FirebaseConfig
        {
            AuthSecret = "NZ8XagMJtYlhQcC1837fZXFy9C57wXiyIVe1FkuU",
            BasePath = "https://projeto-parcial2.firebaseio.com/"
        };
        IFirebaseClient client;

        private int InitIndice;

        private async void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                client = new FireSharp.FirebaseClient(config);
                if (client != null)
                {
                    MessageBox.Show("Conectado ao Servidor", "Sucesso", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch
            {
                MessageBox.Show("Erro ao conectar-se ao servidor,\n tente novamente mais tarde!", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                Application.Exit();
            }

            chart1.Series[0].Points.AddXY(0, 0);
            chart1.Series[1].Points.AddXY(0, 0);
            chart1.Series[2].Points.AddXY(0, 0);

            int i;
            FirebaseResponse response = await client.GetAsync("/Decolagem/Indice/");
            Ind registro = response.ResultAs<Ind>();

            InitIndice = registro.Registro;

            for (i = 1; i <= InitIndice; i++)
            {
                FirebaseResponse response1 = await client.GetAsync("/Decolagem/Registro/" + i.ToString());
                Dados dados = response1.ResultAs<Dados>();

                chart1.Series[0].Points.AddXY(i, int.Parse(dados.Velocidade));
                chart1.Series[1].Points.AddXY(i, int.Parse(dados.Altitude));
                if(int.Parse(dados.Trem) == 1)
                {
                    chart1.Series[2].Points.AddXY(i, 500);
                }
                else
                {
                    chart1.Series[2].Points.AddXY(i, 0);
                }
            }
            timer1.Enabled = true;
        }

        private async void timer1_Tick(object sender, EventArgs e)
        {
            FirebaseResponse response = await client.GetAsync("/Decolagem/Indice/");
            Ind ind = response.ResultAs<Ind>();

            if (ind.Registro > InitIndice)
            {
                InitIndice = ind.Registro;
                FirebaseResponse responseX = await client.GetAsync("Decolagem/Registro/" + ind.Registro.ToString());
                Dados dados = responseX.ResultAs<Dados>();

                chart1.Series[0].Points.AddXY(ind.Registro, int.Parse(dados.Velocidade));
                chart1.Series[1].Points.AddXY(ind.Registro, int.Parse(dados.Altitude));
                if (int.Parse(dados.Trem) == 1)
                {
                    chart1.Series[2].Points.AddXY(ind.Registro, 500);
                }
                else
                {
                    chart1.Series[2].Points.AddXY(ind.Registro,0);
                }
                
            }

        }
    }
}
