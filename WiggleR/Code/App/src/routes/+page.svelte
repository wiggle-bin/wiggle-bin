<script lang="ts">
	import ContourPlot  from '../lib/plots/ContourPlot.svelte'
	import TimelapseVideo  from '../lib/TimelapseVideo.svelte'
</script>

<kor-grid spacing="s">
	<kor-card grid-cols="6" grid-rows="1">
		<TimelapseVideo />
	</kor-card>
	<kor-card grid-cols="6" grid-rows="1">
		<TimelapseVideo />
	</kor-card>
	<kor-card grid-cols="6" grid-rows="1">
		<ContourPlot type="bar" />
	</kor-card>

	<kor-card  label="" grid-cols="6" grid-rows="1">
		<ContourPlot type="scatter" />
	</kor-card>
</kor-grid>