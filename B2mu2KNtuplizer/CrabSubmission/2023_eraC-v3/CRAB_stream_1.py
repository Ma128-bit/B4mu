from CRABClient.UserUtilities import config, getUsername
config = config()

config.General.requestName = 'SkimB4Mu_2023eraC-v3_stream1_Mini'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = '/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/B4muNtuplizer/CrabSubmission/2023_eraC-v3/PatAndTree_cfg.py'

config.Data.inputDataset = '/ParkingDoubleMuonLowMass1/Run2023C-22Sep2023_v3-v1/MINIAOD'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions23/Cert_Collisions2023_eraC_367095_368823_Golden.json'
#config.Data.publication = True
config.Data.outputDatasetTag = 'SkimB4Mu_2023eraC-v3_stream1_Mini'
config.JobType.allowUndistributedCMSSW = True 
config.Site.storageSite = 'T2_IT_Bari'
config.Site.ignoreGlobalBlacklist  = True
