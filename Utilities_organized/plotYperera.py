import ROOT

# Definizione dei dati
x = [0, 1, 2, 3]  # Indici per le label
y = [1.883, 1.533, 1.314, 1.510]  # Valori su y
ex = [0, 0, 0,0]  # Errori su x (zero)
ey = [0.25, 0.27, 0.127, 0.105]  # Errori su y
labels = ["2022", "2023", "2024", "All"]  # Label per l'asse x

# Creazione del TGraphErrors
graph = ROOT.TGraphErrors(len(x))
for i in range(len(x)):
    graph.SetPoint(i, x[i], y[i])
    graph.SetPointError(i, ex[i], ey[i])

# Stile del grafico
graph.SetTitle("Data with Errors")
graph.SetMarkerStyle(21)
graph.SetMarkerSize(1.2)
graph.SetLineColor(ROOT.kBlue + 1)
graph.SetLineWidth(2)

# Creazione del canvas
canvas = ROOT.TCanvas("canvas", "TGraphErrors Example", 800, 600)
canvas.SetGrid()

# Imposta i margini per evitare sovrapposizioni
canvas.SetLeftMargin(0.15)
canvas.SetBottomMargin(0.15)

# Disegna il grafico
graph.Draw("AP")

# Personalizzazione degli assi
xaxis = graph.GetXaxis()
yaxis = graph.GetYaxis()

xaxis.SetTitle("Year")
xaxis.SetTitleSize(0.05)
xaxis.SetTitleOffset(1.2)
xaxis.SetLabelSize(0.045)
xaxis.SetNdivisions(510)  # Riduce il numero di tick

yaxis.SetTitle("Entries/lumi")
yaxis.SetTitleSize(0.05)
yaxis.SetTitleOffset(1.4)
yaxis.SetLabelSize(0.045)

# Imposta le label personalizzate sull'asse x
xaxis.SetLimits(-0.5, 3.5)  # Aggiunge spazio extra
for i, label in enumerate(labels):
    xaxis.ChangeLabel(i + 1, -1, 0)  # Rimuove eventuali valori numerici
    xaxis.SetBinLabel(xaxis.FindBin(x[i]), label)

# Migliora l'aspetto generale
graph.SetMarkerColor(ROOT.kRed + 1)
graph.SetLineColor(ROOT.kRed + 1)
graph.SetLineWidth(2)

# Aggiorna e salva il grafico
canvas.Update()
canvas.SaveAs("tgrapherrors_improved.png")