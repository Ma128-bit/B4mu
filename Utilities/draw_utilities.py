# HELP:
# Run As: plot(histos[], options)
# options:
## Vector (with histos len): SetColor, MarkerColor, MarkerStyle, FillStyle, DrawOpt
## Bool: Fill, Norm, SetLogX, SetLogY, SetLogZ
## String: SetXName, SetYName, SaveAs
## Vectors (with len 2): figsize, SetXRange, SetYRange
## CMSStyle: era (or eras), extra
# Usage example:
## plot([histogram_1, histogram_2, histogram_3], era='2022', extra="Preliminary", SetYName="Events / 5 GeV", SetYRange=[0.0001, 0.1], Fill=True, Norm= True, SaveAs="pippo.png", SetColor=[8, 9, 46], DrawOpt=["histo", "H", "P"], SetLogY=True)

from ROOT import TCanvas, TLine, std, TLatex, TLegend, TPad, kWhite
import numpy as np
import CMSStyle
CMSStyle.setTDRStyle()

def plot(histo, **kwargs):
    options = {
        'SetColor': [i + 1 for i in range(len(histo))],
        'MarkerColor': [],
        'MarkerStyle': [20 for i in range(len(histo))],
        'FillStyle': [i + +3004 for i in range(len(histo))],
        'DrawOpt': ["hist" for i in range(len(histo))],
        'Fill': False,
        'Norm': False,
        'figsize': [900, 600],
        'SetXName': "",
        'SetYName': "",
        'SetLogX': False,
        'SetLogY': False,
        'SetLogZ': False,
    }
    for key in options:
        if key in kwargs:
            options[key] = kwargs.get(key)

    if len(options['MarkerColor'])==0:
        options['MarkerColor'] = options['SetColor']
        
    for i in range(1,len(histo)):
        options['DrawOpt'][i]=options['DrawOpt'][i]+" same"
    
    canvas = TCanvas("canvas", "Canvas", options['figsize'][0], options['figsize'][1])
    
    histo[0].GetXaxis().SetTitle(options['SetXName'])
    histo[0].GetYaxis().SetTitle(options['SetYName'])

    for i in range(len(histo)):
        histo[i].SetLineWidth(2)
        histo[i].SetLineColor(options['SetColor'][i])
        histo[i].SetMarkerColor(options['MarkerColor'][i])
        histo[i].SetMarkerStyle(options['MarkerStyle'][i])
        if(options['Fill']):
            histo[i].SetFillColor(options['SetColor'][i])
            histo[i].SetFillStyle(options['FillStyle'][i])
        if(options['Norm']):
            histo[i].Scale(1/histo[i].Integral(0, histo[i].GetNbinsX() + 1))
        histo[i].Draw(options['DrawOpt'][i])
        
    if 'SetXRange' in kwargs:
        x_range = kwargs.get('SetXRange')
        histo[0].GetXaxis().SetRangeUser(x_range[0],x_range[1])

    if 'SetYRange' in kwargs:
        y_range = kwargs.get('SetYRange')
        histo[0].GetYaxis().SetRangeUser(y_range[0],y_range[1])

    if options['SetLogX'] == True:
        canvas.SetLogx();
    if options['SetLogY'] == True:
        canvas.SetLogy();
    if options['SetLogZ'] == True:
        canvas.SetLogz();
    
    if 'era' in kwargs:
        e = kwargs.get('era')
        if 'extra' in kwargs:
            ex = kwargs.get('extra')
            CMSStyle.setCMSLumiStyle(canvas,0, era=e, extra=ex)
        else:
            CMSStyle.setCMSLumiStyle(canvas,0, era=e)
    elif 'eras' in kwargs:
        e = kwargs.get('eras')
        if 'extra' in kwargs:
            ex = kwargs.get('extra')
            CMSStyle.setCMSLumiStyle(canvas,0, eras=e, extra=ex)
        else:
            CMSStyle.setCMSLumiStyle(canvas,0, eras=e)
    else:
        CMSStyle.setCMSLumiStyle(canvas,0)
    
    if 'SaveAs' in kwargs:
        out_name = kwargs.get('SaveAs')
        
    canvas.Draw()
    canvas.SaveAs(out_name)
    canvas.Clear()
    canvas.Close()
    return True
    

class ROOTDrawer:
    def __init__(self, **kwargs):
        options = {
            'SetGridx': False,
            'SetGridy': False,
            'figsize': [800, 600],
            'SetName': "canvas",
            'SetTitle': "Canvas",
            'SetLogX': False,
            'SetLogY': False,
            'SetLogZ': False,
            'SetXRange': [None, None],
            'SetYRange': [None, None],
        }
        for key in options:
            if key in kwargs:
                options[key] = kwargs.get(key)
        
        canvas = TCanvas(options['SetName'], options['SetTitle'], options['figsize'][0], options['figsize'][1])

        if options['SetGridx']:
            canvas.SetGridx()
        if options['SetGridy']:
            canvas.SetGridy()

        self.log = [options['SetLogX'], options['SetLogY'], options['SetLogZ']]
        
        if options['SetLogX'] == True:
            canvas.SetLogx();
        if options['SetLogY'] == True:
            canvas.SetLogy();
        if options['SetLogZ'] == True:
            canvas.SetLogz();
                
        self.canvas = canvas
        self.histos = []
        self.lines = []
        
        if 'SetXRange' in kwargs:
            self.FixXRange = True
        else:
            self.FixXRange = False
        
        if 'SetYRange' in kwargs:
            self.FixYRange = True
        else:
            self.FixYRange = False

        self.XRange = options['SetXRange']
        self.YRange = options['SetYRange']
        self.Legend = None
        self.pullhisto = []
        self.pullline = []
        self.dopull = False
        self.grid = [options['SetGridx'], options['SetGridy']]

    def HaddTH1(self, histo_temp, **kwargs):
        histo = histo_temp.Clone()
        options = {
            'Color': 1,
            'LineWidth': 1,
            'LineStyle': 1,
            'MarkerColor': 1,
            'MarkerStyle': 20,
            'FillStyle': 3004 ,
            'DrawOpt': "hist",
            'label': "",
            'Fill': False,
            'Norm': False,
            'SetXName': "",
            'SetYName': "",
            'pull': False,
        }
        for key in options:
            if key in kwargs:
                options[key] = kwargs.get(key)

        histo.GetXaxis().SetTitle(options['SetXName'])
        histo.GetYaxis().SetTitle(options['SetYName'])

        histo.SetLineWidth(2)
        histo.SetLineColor(options['Color'])
        histo.SetLineWidth(options['LineWidth'])
        histo.SetLineStyle(options['LineStyle'])
        histo.SetMarkerColor(options['MarkerColor'])
        histo.SetMarkerStyle(options['MarkerStyle'])
        if(options['Fill']):
            histo.SetFillColor(options['Color'])
            histo.SetFillStyle(options['FillStyle'])
        if(options['Norm']):
            histo.Scale(1/histo.Integral(0, histo.GetNbinsX() + 1))
        
        if self.FixYRange == False:
            if(self.YRange[1] is None or histo.GetMaximum()>self.YRange[1]):
                self.YRange[1] = histo.GetMaximum()

            if(self.YRange[0] is None or histo.GetMinimum()<self.YRange[0]):
                if(self.log[1] == False):
                    self.YRange[0] = histo.GetMinimum()
                elif(self.log[1] == True and histo.GetMinimum()>0):
                     self.YRange[0] = histo.GetMinimum()
                elif(self.log[1] == True and histo.GetMinimum()<=0):
                    num_bins = histo.GetNbinsX()
                    ymin = None
                    for i in range(1, num_bins + 1):
                        y_value = histo.GetBinContent(i)
                        if y_value != 0:
                            if ymin is None or y_value < ymin:
                                ymin = y_value
                    self.YRange[0] = ymin
                    
        
        if self.FixXRange == False:
            if(self.XRange[1] is None or histo.GetXaxis().GetXmax()>self.XRange[1]):
                self.XRange[1] = histo.GetXaxis().GetXmax()
            if(self.XRange[0] is None or histo.GetXaxis().GetXmin()<self.XRange[0]):
                self.XRange[0] = histo.GetXaxis().GetXmin()    
            
        out = [histo, options['label'], options['DrawOpt']]
        if(options['pull']==False):
            if(len(self.histos)>0):
                out[2] = out[2] + "same"
            self.histos.append(out)
        else:
            if(len(self.pullhisto)>0):
                out[2] = out[2] + "same"
            self.pullhisto.append(out)

    def DefTLine(self, **kwargs):
        options = {
            'Color': 1,
            'LineWidth': 2,
            'LineStyle': 2,
            'label': "",
            'Orientation': -1, #0 for vertical 1 for horizontal
            'X': 0,
            'Y': 0,
            'X_0': 0,
            'X_1': 0,
            'Y_0': 0,
            'Y_1': 0,
            'pull': False,
        }
        for key in options:
            if key in kwargs:
                options[key] = kwargs.get(key)

        if self.log[1] == True:
            const = 2
        else:
            const =1.1

        if options['Orientation'] == 0:
            options['X_0'] = options['X']
            options['X_1'] = options['X']
            options['Y_0'] = self.YRange[0]
            options['Y_1'] = const*self.YRange[1]
        elif options['Orientation'] == 1:
            options['X_0'] = self.XRange[0]
            options['X_1'] = self.XRange[1]
            options['Y_0'] = options['Y']
            options['Y_1'] = options['Y']

        line = TLine(options['X_0'] , options['Y_0'], options['X_1'], options['Y_1'])
        #print("Line: ", options['X_0'] , " ", options['Y_0'], " ", options['X_1'], " ", options['Y_1'])
        line.SetLineColor(options['Color'])
        line.SetLineWidth(options['LineWidth'])
        line.SetLineStyle(options['LineStyle'])
        
        out= [line, options['label']]
        if(options['pull']==False):
            self.lines.append(out)
        else:
            self.pullline.append(out)
    
    def MakeLegend(self):
        #legend size
        dx_l = 0.45
        dy_l = 0.1 * len(self.histos)
        
        #automatically place legend depending on peak position
        proba = np.array([0.0, 0.5, 1.0])
        bool=[]
        for i in range(len(self.histos)):
            xi = np.array([0.0, 0.0, 0.0])
            self.histos[i][0].GetQuantiles(3,xi,proba)
            rangei = self.histos[i][0].GetXaxis().GetXmax() - self.histos[i][0].GetXaxis().GetXmin()
            booli = (self.histos[i][0].GetXaxis().GetXmax() - xi[1])/rangei > 0.5
            bool.append(booli)
            
        isleft = sum(bool)/len(bool)
        
        if isleft==1: #peak is left for all hists
            x1_l = 0.60;
        elif isleft==0: #peak is right for all hists
            x1_l = 0.20;
        else: #place legend in the center
            x1_l = 0.40 - dx_l/2.0;
        
        y1_l = 0.9 - 0.1 * len(self.histos)
        leg = TLegend(x1_l,y1_l,x1_l+dx_l,y1_l+dy_l)
        leg.SetBorderSize(0)
        leg.SetFillColor(0)
        leg.SetFillStyle(0)
        leg.SetTextFont(42)
        leg.SetTextSize(0.035)
        
        for i in range(len(self.histos)):
            opt = ""
            if(self.histos[i][1]!=""):
                if "Histo" in self.histos[i][2] or "h" in self.histos[i][2]:
                    opt = opt +"F"
                if "P" in self.histos[i][2] or "p" in self.histos[i][2]:
                    opt = opt +"lp"
                if "E" in self.histos[i][2]:
                    opt = opt +"e"
                leg.AddEntry(self.histos[i][0],self.histos[i][1],opt)

        for i in range(len(self.lines)):
            if(self.lines[i][1]!=""):
                leg.AddEntry(self.lines[i][0],self.lines[i][1],"L")
                    
        self.Legend = leg

    def HaddPull(self, **kwargs):
        options = {
            'SetGridx': False,
            'SetGridy': False,
            'SetLogX': False,
            'SetLogY': False,
            'SetLogZ': False,
            'xlow': 0.,
            'ylow': 0.,
            'xup': 1.,
            'yup': 0.3,
            'YRange': [-1,3]
        }
        for key in options:
            if key in kwargs:
                options[key] = kwargs.get(key)

        self.pullYRange = options['YRange']
        self.dopull=True
        self.logpull = [options['SetLogX'], options['SetLogY'], options['SetLogZ']]
        self.canvas.cd()
        self.pad1 = TPad("pad1", "pad1", options['xlow'], options['yup'], options['xup'], 0.95)
        self.pad1.SetTopMargin(0.01)
        if self.log[0] == True:
            self.pad1.SetLogx()
        if self.log[1] == True:
            self.pad1.SetLogy()
        if self.log[2] == True:
            self.pad1.SetLogz()
        if(self.grid[0]):
            self.pad1.SetGridx()
        if(self.grid[1]):
            self.pad1.SetGridy()
        self.pad1.Draw()
        
        self.canvas.cd()
        self.pad2 = TPad("pad2", "pad2", options['xlow'], options['ylow'], options['xup'], options['yup'])
        self.pad2.SetBottomMargin(0.2)
        if options['SetLogX'] == True:
            self.pad2.SetLogx()
        if options['SetLogY'] == True:
            self.pad2.SetLogy()
        if options['SetLogZ'] == True:
            self.pad2.SetLogz()
        if(options['SetGridx']):
            self.pad2.SetGridx()
        if(options['SetGridy']):
            self.pad2.SetGridy()
        self.pad2.Draw()
        
    def Save(self, name, **kwargs):
        self.canvas.cd()
        
        if self.dopull:
            self.pad1.cd() 
            
        if len(self.histos) > 0:
            if self.log[1] == True:
                const = 2
            else:
                const =1.1
            self.histos[0][0].GetXaxis().SetRangeUser(self.XRange[0], self.XRange[1])
            self.histos[0][0].GetYaxis().SetRangeUser(self.YRange[0], const * self.YRange[1])
            for i in range(len(self.histos)):
                self.histos[i][0].Draw(self.histos[i][2])
                
        if len(self.lines) > 0:
            #self.canvas.cd()
            for i in range(len(self.lines)):
                self.lines[i][0].Draw("same")

        if self.Legend is not None:
            self.Legend.Draw("same")
            
        if self.dopull:
            self.pad2.cd() 
            
        if len(self.pullhisto) > 0:
            if self.logpull[1] == True:
                const = 2
            else:
                const =1.1
            self.pullhisto[0][0].GetXaxis().SetRangeUser(self.XRange[0], self.XRange[1])
            self.pullhisto[0][0].GetXaxis().SetLabelSize(0.09);
            self.pullhisto[0][0].GetYaxis().SetLabelSize(0.09);
            self.pullhisto[0][0].GetXaxis().SetTitleSize(0.09);
            self.pullhisto[0][0].GetYaxis().SetTitleSize(0.09);
            self.pullhisto[0][0].GetYaxis().SetTitleOffset(self.pullhisto[0][0].GetYaxis().GetTitleOffset()/2);
            self.pullhisto[0][0].GetYaxis().SetRangeUser(self.pullYRange[0], const * self.pullYRange[1])
            for i in range(len(self.pullhisto)):
                self.pullhisto[i][0].Draw(self.pullhisto[i][2])
                
        if len(self.pullline) > 0:
            #self.canvas.cd()
            for i in range(len(self.pullline)):
                self.pullline[i][0].Draw("same")
        
        if 'era' in kwargs:
            e = kwargs.get('era')
            if 'extra' in kwargs:
                ex = kwargs.get('extra')
                CMSStyle.setCMSLumiStyle(self.canvas,0, era=e, extra=ex)
            else:
                CMSStyle.setCMSLumiStyle(self.canvas,0, era=e)
        elif 'eras' in kwargs:
            e = kwargs.get('eras')
            if 'extra' in kwargs:
                ex = kwargs.get('extra')
                CMSStyle.setCMSLumiStyle(self.canvas,0, eras=e, extra=ex)
            else:
                CMSStyle.setCMSLumiStyle(self.canvas,0, eras=e)
        else:
            CMSStyle.setCMSLumiStyle(self.canvas,0)
        
        self.canvas.Update()
        self.canvas.SaveAs(name)
    
    def Delete(self,  **kwargs):
        self.canvas.Clear()
        self.canvas.Close()
        del self.canvas
        del self.histos
        del self.YRange
        del self.XRange
        del self.log
        del self.Legend
        del self.lines
        del self.dopull
        del self.pullYRange
        del self
    
