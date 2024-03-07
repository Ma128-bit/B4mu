from CRABClient.UserUtilities import config, getUsername
config = config()

config.General.requestName = 'SkimB4Mu_2022eraF_stream1_Mini'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = '/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/B4muNtuplizer/CrabSubmission/2022_eraF/PatAndTree_cfg.py'

config.Data.inputDataset = '/ParkingDoubleMuonLowMass1/Run2022F-22Sep2023-v1/MINIAOD'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/Cert_Collisions2022_eraF_360390_362167_Golden.json'
#config.Data.publication = True
config.Data.outputDatasetTag = 'SkimB4Mu_2022eraF_stream1_Mini'
config.JobType.allowUndistributedCMSSW = True 
config.Site.storageSite = 'T2_IT_Bari'
config.Site.ignoreGlobalBlacklist  = True
