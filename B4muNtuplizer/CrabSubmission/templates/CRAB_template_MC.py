from CRABClient.UserUtilities import config, getUsername
config = config()

config.General.requestName = 'SkimB4Mu_YEAR_ERANAME_Mini'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'

config.JobType.psetName = 'FILE_TO_SUBMIT_PATH'

config.Data.inputDataset = 'MC_DATASET'
config.Data.inputDBS = 'INPUT_TYPE'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
#config.Data.publication = True
config.Data.outputDatasetTag = 'SkimB4Mu_YEAR_ERANAME_Mini'
config.JobType.allowUndistributedCMSSW = True 
config.Site.storageSite = 'T2_IT_Bari'
config.Site.ignoreGlobalBlacklist  = True
