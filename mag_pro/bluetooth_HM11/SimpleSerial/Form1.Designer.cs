namespace SimpleSerial
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonStart = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.Save_BT = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.timer4 = new System.Windows.Forms.Timer(this.components);
            this.timer5 = new System.Windows.Forms.Timer(this.components);
            this.PORT_LS = new System.Windows.Forms.ComboBox();
            this.button2 = new System.Windows.Forms.Button();
            this.connect_bt = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(874, 95);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(98, 23);
            this.buttonStart.TabIndex = 0;
            this.buttonStart.Text = "OPEN COM";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(847, 438);
            this.textBox1.TabIndex = 2;
            this.textBox1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox1_KeyPress);
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(874, 64);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(98, 25);
            this.button1.TabIndex = 3;
            this.button1.Text = "SCAN PORT";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(905, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "PORT";
            // 
            // Save_BT
            // 
            this.Save_BT.Location = new System.Drawing.Point(874, 182);
            this.Save_BT.Name = "Save_BT";
            this.Save_BT.Size = new System.Drawing.Size(98, 23);
            this.Save_BT.TabIndex = 8;
            this.Save_BT.Text = "SAVE LOG";
            this.Save_BT.UseVisualStyleBackColor = true;
            this.Save_BT.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(874, 153);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(98, 23);
            this.button3.TabIndex = 9;
            this.button3.Text = "OPEN LOG";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // timer4
            // 
            this.timer4.Interval = 1000;
            this.timer4.Tick += new System.EventHandler(this.timer4_Tick);
            // 
            // timer5
            // 
            this.timer5.Tick += new System.EventHandler(this.timer5_Tick);
            // 
            // PORT_LS
            // 
            this.PORT_LS.FormattingEnabled = true;
            this.PORT_LS.Location = new System.Drawing.Point(874, 37);
            this.PORT_LS.Name = "PORT_LS";
            this.PORT_LS.Size = new System.Drawing.Size(98, 21);
            this.PORT_LS.TabIndex = 4;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(874, 397);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(98, 53);
            this.button2.TabIndex = 10;
            this.button2.Text = "Clear";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // connect_bt
            // 
            this.connect_bt.Location = new System.Drawing.Point(874, 124);
            this.connect_bt.Name = "connect_bt";
            this.connect_bt.Size = new System.Drawing.Size(98, 23);
            this.connect_bt.TabIndex = 11;
            this.connect_bt.Text = "CONNECT";
            this.connect_bt.UseVisualStyleBackColor = true;
            this.connect_bt.Click += new System.EventHandler(this.connect_bt_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(984, 462);
            this.Controls.Add(this.connect_bt);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.Save_BT);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.PORT_LS);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.buttonStart);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximumSize = new System.Drawing.Size(1000, 500);
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.TextBox textBox1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button Save_BT;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Timer timer4;
        private System.Windows.Forms.Timer timer5;
        private System.Windows.Forms.ComboBox PORT_LS;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button connect_bt;
    }
}

