-------------------------------------------------------------------------------
--  External Dependancies
-------------------------------------------------------------------------------

function VendorCxxopts()
	includedirs "%{wks.location}/vendor/cxxopts/include"
	defines ""
end


function VendorColoredCout()
	includedirs "%{wks.location}/vendor/colored-cout"
	links "vendor.colored-cout"
end
