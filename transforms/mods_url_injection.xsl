<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes" />
    <xsl:param name="resource_url"/>

    <xsl:template match="node()">
        <xsl:copy>
            <xsl:copy-of select="@* | *"/>
            <identifier type="uri"><xsl:value-of select="$resource_url"/></identifier>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
