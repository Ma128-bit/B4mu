import ROOT

# Apri il file di input e il TTree
input_file = ROOT.TFile.Open("input_file.root", "READ")
input_tree = input_file.Get("treeName")

# Variabili per leggere e scrivere
ul_value = ROOT.Long()  # unsigned long
ui_value = ROOT.UInt()  # unsigned int

# Imposta il branch di input
input_tree.SetBranchAddress("nPileUpInt", ul_value)

# Crea il file di output e un nuovo TTree
output_file = ROOT.TFile.Open("output_file.root", "RECREATE")
output_tree = ROOT.TTree("newTreeName", "Tree with unsigned int")

# Crea il nuovo branch
output_tree.Branch("nPileUpInt_new", ui_value, "nPileUpInt_new/i")

# Loop sugli eventi
n_entries = input_tree.GetEntries()
for i in range(n_entries):
    input_tree.GetEntry(i)
    ui_value = ul_value  # Conversione implicita
    output_tree.Fill()

# Salva e chiudi i file
output_file.Write()
output_file.Close()
input_file.Close()
