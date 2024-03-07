from CRABClient.UserUtilities import config, getUsername
config = config()

config.General.requestName = 'SkimB4Mu_2022eraG_stream0_Mini'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = '/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/B4muNtuplizer/CrabSubmission/2022_eraG/PatAndTree_cfg.py'

config.Data.inputDataset = '/ParkingDoubleMuonLowMass0/Run2022G-22Sep2023-v1/MINIAOD'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/Cert_Collisions2022_eraG_362433_362760_Golden.json'
#config.Data.publication = True
config.Data.outputDatasetTag = 'SkimB4Mu_2022eraG_stream0_Mini'
config.JobType.allowUndistributedCMSSW = True 
config.Site.storageSite = 'T2_IT_Bari'
config.Site.ignoreGlobalBlacklist  = True
