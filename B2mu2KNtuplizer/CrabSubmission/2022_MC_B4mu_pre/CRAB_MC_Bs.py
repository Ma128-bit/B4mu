from CRABClient.UserUtilities import config, getUsername
config = config()

config.General.requestName = 'SkimB4Mu_2022_MC_B4mu_pre_Bs_Mini'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = '/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/B4muNtuplizer/CrabSubmission/2022_MC_B4mu_pre/PatAndTree_cfg.py'

config.Data.inputDataset = '/Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/Run3Summer22MiniAODv4-130X_mcRun3_2022_realistic_v5-v3/MINIAODSIM'
config.Data.allowNonValidInputDataset = True
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
#config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 300
#config.Data.publication = True
config.Data.outputDatasetTag = 'SkimB4Mu_2022_MC_B4mu_pre_Bs_Mini'
config.JobType.allowUndistributedCMSSW = True 
config.Site.storageSite = 'T2_IT_Bari'
config.Site.ignoreGlobalBlacklist  = True
