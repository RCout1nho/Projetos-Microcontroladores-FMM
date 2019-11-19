using System;
using System.Windows.Forms;
using FireSharp.Response;
using FireSharp.Config;
using FireSharp.Interfaces;

namespace Projeto_Parcial_1
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        GraphicsForm gf = new GraphicsForm();

        IFirebaseConfig config = new FirebaseConfig
        {
            BasePath = "https://projeto-parcial1x.firebaseio.com/",
            AuthSecret = "AaHaWVLuMRlT3wJwX7PCXDMmgFKw14M8KFqk07BS"
        };

        IFirebaseClient client;

        private async void MainForm_Load(object sender, EventArgs e)
        {
            client = new FireSharp.FirebaseClient(config);

            if (client != null)
            {
                MessageBox.Show("Conectado ao Servidor Firebase", "Sucesso", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Erro ao conectar-se ao servidor,\n tente novamente mais tarde!", "Erro", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                Application.Exit();
            }

            var resposta = new RespostaClass
            {
                LDR = false,
                Pot = false
            };
            FirebaseResponse response = await client.SetAsync("/Respostas/", resposta);
            timer1.Enabled = true;
        }

        private async void BtnLdr_Click(object sender, EventArgs e)
        {
            var solicitacao = new Solicitacao
            {
                LDR = true,
                Pot = false
            };

            SetResponse response = await client.SetAsync("/Solicitacoes/",solicitacao);
        }

        private async void BtnPot_Click(object sender, EventArgs e)
        {
            var solicitacao = new Solicitacao
            {
                LDR = false,
                Pot = true
            };

            SetResponse response = await client.SetAsync("/Solicitacoes/", solicitacao);
        }

        GraphClass graph = new GraphClass();
        private async void LinkLb_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            FirebaseResponse responseI = await client.GetAsync("/Indexes/");
            IndexesClass indexes = responseI.ResultAs<IndexesClass>();

            int index = indexes.LDR;
            graph.ldrIndex = index;

            for(int i = 1; i <= index; i++)
            {
                FirebaseResponse responseD = await client.GetAsync("/Sensores/LDR/" + i.ToString());
                DataClass data = responseD.ResultAs<DataClass>();

                graph.setldrValue(i, data.Valor);
            }

            index = indexes.Pot;
            graph.potIndex = index;

            for (int i = 1; i <= index; i++)
            {
                FirebaseResponse responseD = await client.GetAsync("/Sensores/Pot/" + i.ToString());
                DataClass data = responseD.ResultAs<DataClass>();

                graph.setpotValue(i, data.Valor);
            }

            gf.ShowDialog();
        }

        private async void Timer1_Tick(object sender, EventArgs e)
        { 
            FirebaseResponse response = await client.GetAsync("/Respostas");
            RespostaClass resposta = response.ResultAs<RespostaClass>();

            FirebaseResponse responseI = await client.GetAsync("/Indexes/");
            IndexesClass indexes = responseI.ResultAs<IndexesClass>();

            if (resposta.Pot)
            {                
                string index = (indexes.Pot).ToString();

                FirebaseResponse responseP = await client.GetAsync("/Sensores/Pot/" + index);
                DataClass data = responseP.ResultAs<DataClass>();

                lbDataPot.Text = data.Data;
                lbHoraPot.Text = data.Hora;
                lbValorPot.Text = data.Valor;

                resposta.LDR = false;
                resposta.Pot = false;

                FirebaseResponse resposeRR = await client.SetAsync("/Respostas", resposta);
            }
            if (resposta.LDR)
            {
                string index = (indexes.LDR).ToString();

                FirebaseResponse responseD = await client.GetAsync("/Sensores/LDR/" + index);
                DataClass data = responseD.ResultAs<DataClass>();

                lbDataLdr.Text = data.Data;
                lbHoraLdr.Text = data.Hora;
                lbValorLdr.Text = data.Valor;

                resposta.LDR = false;
                resposta.Pot = false;

                FirebaseResponse resposeRR = await client.SetAsync("/Respostas", resposta);
            }            
        }
    }
}
