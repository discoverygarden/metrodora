<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/dc/">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes" />
    <xsl:param name="resource_url"/>

    <xsl:template match="node()">
        <xsl:copy>
            <xsl:copy-of select="@* | *"/>
            <dc:identifier><xsl:value-of select="$resource_url"/></dc:identifier>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
