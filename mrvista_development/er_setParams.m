function er_setParams(view,params,scans,dt)
% er_setParams(view,params,[scans],[dt]):
%
% Set the event-related analysis params
% for the current scan in dataTYPES'
% eventAnalysisParams subfield. 
% 
% params should be a struct with appropriate
% fields (anything in params will be added 
% to the eventAnalysisParams field). See 
% er_getParams for examples of these fields.
%
% A lot of the newer params are used for
% er_chopTSeries, and the time course UI.
%
% scan defaults to view's current scan.
%
% ras 12/04
% dar 03/07 - sort parameter list by alphabet - prevents concatenation
% errors.
global dataTYPES;

if notDefined('scans') || notDefined('dt') 
    [curScans curDt] = er_getScanGroup(view); %%% changing to deal with undefined 'scans' and 'dt' separately (RD)
end
if notDefined('scans')
    scans = curScans;
end
if notDefined('dt')
    dt = curDt;
end
if notDefined('params')
    % When params is empty, we need to do something ... maybe just return?
    % This happens when there is a user cancel on the main window
    return;
end

% make sure we have the data type name and #
if ischar(dt), dt = existDataType(dt); end
dtName = dataTYPES(dt).name;

allParams = dataTYPES(dt).eventAnalysisParams;

fnames = fieldnames(params);

for scan = scans
	for i = 1:length(fnames)
        if ~isfield(allParams(scan),fnames{i})
            % initialize param for all scans
            for s = 1:length(allParams)
                allParams(s).(fnames{i}) = [];
            end
        end
        
        allParams(scan).(fnames{i}) = params.(fnames{i});
	end
end

dataTYPES(dt).eventAnalysisParams = sortFields(allParams);

% let's go ahead and save the changes
global HOMEDIR
mrSessFile = fullfile(HOMEDIR,'mrSESSION.mat');
if exist(mrSessFile,'file')
    save(mrSessFile,'dataTYPES','-append');
end

fprintf('Updated Event Analysis Params for %s scans %s.\n',dtName,num2str(scans));

return